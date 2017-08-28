//
// Created by devmaany on 08.08.17.
//

#ifndef DYNCAM_NETCAMAGGREGATOR_H
#define DYNCAM_NETCAMAGGREGATOR_H
#include "TCPCommons.h"
#include "TCPClient.h"
#include "../BaseCam.h"
class NetCam : public BaseCam {
private:
    boost::asio::io_service m_IOService;
    std::set<BaseCam::StreamCapabilities> m_StreamCapabilities;
public:
    void FindServer();
    TCPClient::pointer Connect(char const * IPAddress,int port,std::vector<BaseCam::StreamCapabilities> requestStreams);
    bool Disconnect(boost::asio::ip::tcp::endpoint);
    std::set<BaseCam::StreamCapabilities> getStreamCapabilities() override;
};
#endif //DYNCAM_NETCAMAGGREGATOR_H
