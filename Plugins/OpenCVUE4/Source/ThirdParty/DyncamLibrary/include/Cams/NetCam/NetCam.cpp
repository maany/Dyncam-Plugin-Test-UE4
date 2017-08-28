//
// Created by devmaany on 08.08.17.
//

#include "NetCam.h"

// ------------------------------------------
//         NetCam (the client)
// ------------------------------------------

TCPClient::pointer NetCam::Connect(char const* IPAddress, int port, std::vector<BaseCam::StreamCapabilities> requestStreams) {
    boost::asio::ip::tcp::endpoint serverEndpoint(boost::asio::ip::address::from_string(std::string(IPAddress)),port);
    try {
        std::cout << "[NetCam] : " << " Trying to find the server" << std::endl;
        TCPClient::pointer client = TCPClient::Create(m_IOService, this);
        client->GetSocket().connect(serverEndpoint);
        client->ReceiveHandshakeRequest();
        client->SendHandshakeAck(requestStreams);
        for_each(requestStreams.begin(), requestStreams.end(),[&](BaseCam::StreamCapabilities requestStream){
            m_StreamCapabilities.insert(requestStream);
        });

        // what's done here?
        while (client->GetSocket().is_open()) {
            client->Execute();
        }
        return client;
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return nullptr;
    }
}
std::set<BaseCam::StreamCapabilities> NetCam::getStreamCapabilities(){
    return m_StreamCapabilities;
}