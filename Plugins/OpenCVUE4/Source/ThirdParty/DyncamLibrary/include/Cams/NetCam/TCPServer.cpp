//
// Created by mayank on 8/22/17.
//

#include "TCPServer.h"

// ------------------------------------------//
//                TCPServer                  //
// ------------------------------------------//

TCPServer::TCPServer(boost::asio::io_service &ioService) : m_Socket(ioService) {

}

boost::shared_ptr<TCPServer> TCPServer::Create(boost::asio::io_service &io_service) {
    return pointer(new TCPServer(io_service));
}

boost::asio::ip::tcp::socket &TCPServer::GetSocket() {
    return m_Socket;
}

void TCPServer::Execute(cv::Mat mat, BaseCam::StreamCapabilities streamType) {
    SendAsStream(mat, streamType);
}

void TCPServer::SendHandshakeRequest(std::vector<TCPCommons::NetCamCVMatParams> netCamCVMatParams) {
    boost::asio::streambuf handshakeBuf;
    std::ostream handshakeRequestStream(&handshakeBuf);
    handshakeRequestStream << netCamCVMatParams.size() << "\n";
    for (uint i = 0; i < netCamCVMatParams.size(); i++) {
        const char *data = netCamCVMatParams.at(i).ToString();
        std::cout << "[Handshake Request] : Writing : " << data << std::endl;
        handshakeRequestStream << data << "\n";
        free((void *) data);
    }
    handshakeRequestStream << "\r\n";
    boost::asio::write(m_Socket, handshakeBuf);
}

std::vector<BaseCam::StreamCapabilities> TCPServer::ReceiveHandshakeAck() {
    std::vector<BaseCam::StreamCapabilities> requestedStreams;
    boost::asio::streambuf handshakeAckBuf;
    boost::asio::read_until(m_Socket, handshakeAckBuf, "\r\n");
    std::istream handshakeAckStream(&handshakeAckBuf);
    unsigned int noOfRequestedStreams;
    handshakeAckStream >> noOfRequestedStreams;
    handshakeAckStream.get();
    std::cout << "[TCPConnectionServer] : No of Requested Streams : " << noOfRequestedStreams << std::endl;
    for (uint i = 0; i < noOfRequestedStreams; i++) {
        std::string requestedStreamAsString;
        std::getline(handshakeAckStream, requestedStreamAsString);
        int requstedStream = atoi(requestedStreamAsString.data());
        if (requstedStream == static_cast<int>(BaseCam::StreamCapabilities::DEPTH)) {
            std::cout << "[TCPConnectionServer] : Requested Depth" << std::endl;
            requestedStreams.push_back(BaseCam::StreamCapabilities::DEPTH);
        } else if (requstedStream == static_cast<int>(BaseCam::StreamCapabilities::COLOR)) {
            requestedStreams.push_back(BaseCam::StreamCapabilities::COLOR);
            std::cout << "[TCPConnectionServer] : Requested Color" << std::endl;
        } else if (requstedStream == static_cast<int>(BaseCam::StreamCapabilities::IR)) {
            requestedStreams.push_back(BaseCam::StreamCapabilities::IR);
            std::cout << "[TCPConnectionServer] : Requested IR" << std::endl;
        }
    }
    return requestedStreams;
}


void TCPServer::SendAsStream(cv::Mat &mat, BaseCam::StreamCapabilities streamType) {
    boost::asio::streambuf request;
    std::ostream request_stream(&request);

    boost::asio::streambuf matbuf;
    std::ostream data_stream(&matbuf);
    // Send streamType
    int type = static_cast<int>(streamType);
    std::cout << "[Socket Server] : Type = " << type << "\n";
    request_stream << type << "\n";

    //Send timestamp
    long time = (boost::posix_time::second_clock::universal_time() - TCPCommons::Util::EPOCH).total_milliseconds();
    std::cout << "[Socket Server] : TimeStamp = " << time << "\n";
    request_stream << time << "\n";
    request_stream << "\r\n";
    //Send matrix in next go.
    if (mat.isContinuous())
        data_stream.write((char *) mat.data, TCPCommons::Util::GetCVMatSize(mat));
    else {
        std::cerr << "Warning: mat is not continuous!" << std::endl;
        size_t rowsz = mat.elemSize() * mat.cols;
        for (int r = 0; r < mat.rows; ++r)
            data_stream.write((char *) mat.ptr<char>(r), rowsz);
    }
    data_stream.flush();
    request_stream.flush();
    {
        std::lock_guard<std::mutex> lock(m_ServerMutex);
        std::cout << "[TCPServer] : Size of request " << request.size() << std::endl;
        boost::asio::write(m_Socket, request);
        std::cout << "[TCPServer] : Size of mat " << matbuf.size() << std::endl;
        boost::asio::write(m_Socket, matbuf);
    }
}

