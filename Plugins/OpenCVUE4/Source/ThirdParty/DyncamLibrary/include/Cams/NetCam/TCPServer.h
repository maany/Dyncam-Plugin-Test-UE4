//
// Created by mayank on 8/22/17.
//

#ifndef DYNCAM_TCPSERVER_H
#define DYNCAM_TCPSERVER_H

#include "TCPCommons.h"
#include "boost/asio.hpp"
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include "../BaseCam.h"

class TCPServer : public boost::enable_shared_from_this<TCPServer> {
protected:
    std::mutex m_ServerMutex;
    boost::asio::ip::tcp::socket m_Socket;
public:

    typedef boost::shared_ptr<TCPServer> pointer;

    static boost::shared_ptr<TCPServer> Create(boost::asio::io_service &io_service);

    TCPServer(boost::asio::io_service &ioService);

    boost::asio::ip::tcp::socket &GetSocket();

    void SendHandshakeRequest(std::vector<TCPCommons::NetCamCVMatParams> netCamCVMatParams);

    std::vector<BaseCam::StreamCapabilities> ReceiveHandshakeAck();

    void Send(cv::Mat &mat);

    void SendAsStream(cv::Mat &mat, BaseCam::StreamCapabilities streamType);

    void Execute(cv::Mat mat, BaseCam::StreamCapabilities streamType);
};

#endif //DYNCAM_TCPSERVER_H
