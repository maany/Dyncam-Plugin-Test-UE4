//
// Created by devmaany on 04.08.17.
//

#ifndef DYNCAM_NETCAM_H
#define DYNCAM_NETCAM_H


// include CppReact
#include <react/Domain.h>
#include "react/Signal.h"
#include <react/Event.h>
#include <react/Observer.h>
#include "../../ReactDomain.h"

// include exceptions
#include "stdexcept"
#include "TCPCommons.h"
#include "TCPServer.h"
#include "../BaseCam.h"

#include "boost/bind.hpp"
class NetCamServer {
private:
    std::vector<TCPCommons::NetCamCVMatParams> m_NetCamCVMatParams;
    boost::asio::io_service m_IOService;
    TCPCommons::Protocol m_Protocol;
    BaseCam *m_SourceCam;
    TCPServer::pointer m_TCPServer;
    unsigned short m_PortNo;
    std::function<BaseCam::StreamCapabilities(cv::Mat &)> IdentifyStreamType;
    boost::asio::ip::tcp::acceptor *m_Acceptor;

    void StartAccept();

    void HandleAccept(TCPServer::pointer newConnection, const boost::system::error_code &error);

    void StreamOverNetwork(D::EventSourceT<cv::Mat> *eventSource);

public:

    NetCamServer(TCPCommons::Protocol protocol, const unsigned short portNo, BaseCam *sourceCam,
                 std::vector<TCPCommons::NetCamCVMatParams> netCamCVMatParams,
                 std::function<BaseCam::StreamCapabilities(cv::Mat &)> identifyStreamType);

    // FIXME: we need to remove the observers in a destructor

    void StartServer();

    void StopServer();
};

#endif //DYNCAM_NETCAM_H
