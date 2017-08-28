//
// Created by devmaany on 04.08.17.
//
#include "NetCamServer.h"
// ------------------------------------------
//            NetCamServer (The server)
// ------------------------------------------

NetCamServer::NetCamServer(TCPCommons::Protocol protocol, const unsigned short portNo, BaseCam *sourceCam,
                           std::vector<TCPCommons::NetCamCVMatParams> netCamCVMatParams,
                           std::function<BaseCam::StreamCapabilities(cv::Mat &)> IdentifyStreamType) {
    this->IdentifyStreamType = IdentifyStreamType;
    m_Protocol = protocol;
    m_PortNo = portNo;
    m_SourceCam = sourceCam;
    m_NetCamCVMatParams = netCamCVMatParams;
    m_TCPServer = TCPServer::Create(m_IOService);
}

void NetCamServer::StartServer() {
    using acceptor = boost::asio::ip::tcp::acceptor;
    using endpoint = boost::asio::ip::tcp::endpoint;

    std::cout << "[NetCamServer] : " << "Starting Server" << std::endl;
    switch (m_Protocol) {
        case TCPCommons::Protocol::IPv4 :
            m_Acceptor = new acceptor(m_IOService, endpoint(boost::asio::ip::tcp::v4(), m_PortNo));
            break;
        case TCPCommons::Protocol::IPv6 :
            m_Acceptor = new acceptor(m_IOService, endpoint(boost::asio::ip::tcp::v6(), m_PortNo));
            break;
        default:
            throw std::runtime_error( "Server side protocol not supported. Must be IPv4 or IPv6.");
    }

    try {
        StartAccept();
    } catch (std::exception &e) {
        std::cout << "[NetCamServer] Error : " << e.what() << std::endl;
    }

    // this is a blocking call
    // TODO: when does it end? I gues i
    m_IOService.run();
}

void NetCamServer::StartAccept() {
    m_TCPServer = TCPServer::Create(m_IOService);
    m_Acceptor->async_accept(m_TCPServer->GetSocket(),
                             boost::bind(&NetCamServer::HandleAccept, this, m_TCPServer,
                                         boost::asio::placeholders::error));

    std::cout << "[NetCamServer] : " << "Listening for incoming NetCam connection" << std::endl;
}

void NetCamServer::HandleAccept(TCPServer::pointer tcpServer,
                                const boost::system::error_code &error) {
    if (!error) {
        std::cout << "[NetCamServer] : " << " Accepted incoming client connection" << std::endl;

        tcpServer->SendHandshakeRequest(m_NetCamCVMatParams);

        std::vector<BaseCam::StreamCapabilities> requestedStreams = tcpServer->ReceiveHandshakeAck();
        for_each(requestedStreams.begin(), requestedStreams.end(), [&](BaseCam::StreamCapabilities requestedStream) {
            if (requestedStream == BaseCam::StreamCapabilities::DEPTH) {
                StreamOverNetwork(&m_SourceCam->DepthEventsSource);
            } else if (requestedStream == BaseCam::StreamCapabilities::COLOR) {
                StreamOverNetwork(&m_SourceCam->ColorEventsSource);
            } else if (requestedStream == BaseCam::StreamCapabilities::IR) {
                StreamOverNetwork(&m_SourceCam->IrEventsSource);
            }
        });
    } else {
        std::cerr << "Handle Accept error code: " << error << std::endl;
    }
}

void NetCamServer::StreamOverNetwork(D::EventSourceT<cv::Mat> *eventSource) {
    Observe(*eventSource, [this](cv::Mat mat) {
        std::cout << "[NetCamServer] : " << "Streaming over Network" << std::endl;
        std::cout << "[NetCamServer] : " << "Rows, Columns, ElemSize" << mat.rows << ", " << mat.cols << ", "
                  << mat.elemSize()
                  << std::endl;
        bool isOpen = m_TCPServer->GetSocket().is_open();
        std::cout << "[NetCamServer] : is socket still open? : " << isOpen << std::endl;
        if (IdentifyStreamType(mat) == BaseCam::StreamCapabilities::DEPTH) {
            std::cout << "[NetCamServer] : Depth" << std::endl;
            cv::imshow("Server Depth", mat);
        } else if (IdentifyStreamType(mat) == BaseCam::StreamCapabilities::COLOR) {
            std::cout << "[NetCamServer] : Color" << std::endl;
            cv::imshow("Server Color", mat);
        }
        m_TCPServer->Execute(mat, IdentifyStreamType(mat));
    });
}
