//
// Created by mayank on 8/22/17.
//

#include "TCPClient.h"

TCPClient::TCPClient(boost::asio::io_service &ioService, BaseCam *netcam) : m_Socket(ioService), m_NetCam(netcam) {

}

boost::shared_ptr<TCPClient> TCPClient::Create(boost::asio::io_service &io_service, BaseCam *netcam) {
    TCPClient *tcpConnectionClient = new TCPClient(io_service, netcam);
    return pointer(tcpConnectionClient);
}

boost::asio::ip::tcp::socket &TCPClient::GetSocket() {
    return m_Socket;
}

void TCPClient::Execute() {
    std::cout << "[TCPConnectionClient] : " << "Ready to Receive " << std::endl;
    ReceiveAsStream();
}

void TCPClient::ReceiveHandshakeRequest() {
    boost::asio::streambuf handshakeResponse;
    boost::asio::read_until(m_Socket, handshakeResponse, "\r\n");
    std::istream handshakeResponseStream(&handshakeResponse);
    unsigned int number_of_streams;
    handshakeResponseStream >> number_of_streams;
    handshakeResponseStream.get();
    std::cout << "number of streams :: " << number_of_streams << std::endl;
    for (uint i = 0; i < number_of_streams; i++) {
        std::string netCamCVMatData;
        std::getline(handshakeResponseStream, netCamCVMatData);
        TCPCommons::NetCamCVMatParams params;
        TCPCommons::NetCamCVMatParams::LoadFromString(params, const_cast<char *>(netCamCVMatData.c_str()));
        m_NetCamCVMatParams[params.streamType] = params;
        if (params.streamType == static_cast<int>(BaseCam::StreamCapabilities::DEPTH)) {
            std::cout << "[TCPConnectionClient] Depth Stream Available : " << netCamCVMatData << std::endl;
        } else if (params.streamType == static_cast<int>(BaseCam::StreamCapabilities::COLOR)) {
            std::cout << "[TCPConnectionClient] Color Stream Available : " << netCamCVMatData << std::endl;
        } else if (params.streamType == static_cast<int>(BaseCam::StreamCapabilities::IR)) {
            std::cout << "[TCPConnectionClient] IR Stream Available : " << netCamCVMatData << std::endl;
        }
    }
}

void TCPClient::SendHandshakeAck(std::vector<BaseCam::StreamCapabilities> requestStreams) {
    boost::asio::streambuf handshakeAckBuf;
    std::ostream handshakeAckStream(&handshakeAckBuf);
    handshakeAckStream << requestStreams.size() << "\n";
    for (uint i = 0; i < requestStreams.size(); i++) {
        int data = static_cast<int>(requestStreams.at(i));
        handshakeAckStream << data << "\n";
    }
    handshakeAckStream << "\r\n";
    boost::asio::write(m_Socket, handshakeAckBuf);
}

void TCPClient::ReceiveAsStream() {
    boost::asio::streambuf response;
    {
        //std::lock_guard<std::mutex> lock(m_ClientTCPEventMutex);
        boost::asio::read(m_Socket, response, boost::asio::transfer_exactly(15));
    }

    std::istream response_stream(&response);
    unsigned int typeVal = -1;
    response_stream >> typeVal;
    response_stream.get();
    long startTime = 0;
    response_stream >> startTime;
    response_stream.get();
    std::cout << "[TCPClient] : Stream Type: " << typeVal << " : " << typeVal << std::endl;
    boost::asio::streambuf matrix;
    std::istream matrix_stream(&matrix);

    TCPCommons::NetCamCVMatParams cvMatParams = m_NetCamCVMatParams[typeVal];
    boost::asio::read(m_Socket, matrix, boost::asio::transfer_exactly(cvMatParams.frameLength));
    cv::Mat mat(cvMatParams.rows, cvMatParams.cols, cvMatParams.cvMatType);
    {
        //std::lock_guard<std::mutex> lock(m_ClientTCPEventMutex);
        matrix_stream.read((char *) mat.data, cvMatParams.frameLength);
        // End Time
        long endTime = (boost::posix_time::second_clock::local_time() - TCPCommons::Util::EPOCH).total_microseconds();
        std::cout << "[TCPClient] : Transmission Delay : " << endTime - startTime << std::endl;
        if (typeVal == static_cast<int>(BaseCam::StreamCapabilities::DEPTH)) {
            std::cout << "[Stream] : Received Depth Packet" << std::endl;
            m_NetCam->DepthEventsSource.Emit(mat);
        } else if (typeVal == static_cast<int>(BaseCam::StreamCapabilities::COLOR)) {
            std::cout << "[Stream] : Received Color Packet" << std::endl;
            m_NetCam->ColorEventsSource.Emit(mat);
        } else if (typeVal == static_cast<int>(BaseCam::StreamCapabilities::IR)) {
            std::cout << "[Stream] : Received Color Packet" << std::endl;
            m_NetCam->IrEventsSource.Emit(mat);
        }
    }
}