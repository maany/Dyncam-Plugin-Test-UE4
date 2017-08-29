//
// Created by mayank on 8/22/17.
//

#ifndef DYNCAM_TCPCLIENT_H
#define DYNCAM_TCPCLIENT_H
#define MAX_SUPPORTED_STREAM_TYPES_PER_NETCAMSERVER 3
THIRD_PARTY_INCLUDES_START
#include "TCPCommons.h"
#include "boost/asio.hpp"
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
THIRD_PARTY_INCLUDES_END
#include "BaseCam.h"
class TCPClient{// : public boost::enable_shared_from_this<TCPClient> {
protected:
	boost::asio::ip::tcp::socket m_Socket;
	TCPCommons::NetCamCVMatParams m_NetCamCVMatParams[MAX_SUPPORTED_STREAM_TYPES_PER_NETCAMSERVER];
	BaseCam *m_NetCam;
	//std::mutex m_ClientTCPEventMutex;
public:
	typedef boost::shared_ptr<TCPClient> pointer;

	static boost::shared_ptr<TCPClient> Create(boost::asio::io_service &io_service, BaseCam *netcam);

	boost::asio::ip::tcp::socket &GetSocket();

	TCPClient(boost::asio::io_service &ioService, BaseCam *netcam);

	virtual void Execute();

	void ReceiveHandshakeRequest();

	void SendHandshakeAck(std::vector<BaseCam::StreamCapabilities> requestStreams);

	void Receive();

	void ReceiveAsStream();

};


#endif //DYNCAM_TCPCLIENT_H
