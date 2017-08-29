//
// Created by mayank on 8/22/17.
//
#ifndef DYNCAM_TCPCOMMONS_H
#define DYNCAM_TCPCOMMONS_H

#include <boost/date_time/posix_time/ptime.hpp>
#include "BaseCam.h"
#include "opencv2/opencv.hpp"

//THIRD_PARTY_INCLUDES_END
class TCPCommons {
public:
	enum Protocol {
		IPv4, IPv6
	};

	/// Represents metainfo for a single cv::Mat(Color, Depth, IR) that are streamed by a NetCamServer to a NetCamClient.
	/// This information is dependent on the type of camera connected to the NetCamServer.
	/// Once the connection between a NetCamServer and NetCam is complete, multiple instances of NetCamCVMatParams are sent by the NetCamServer to the NetCam in the handshake process.
	/// This information is used to properly reconstruct the cv::Mat at the NetCam machine.
	class NetCamCVMatParams {
	public:
		NetCamCVMatParams();

		NetCamCVMatParams(BaseCam::StreamCapabilities streamType, int rows, int cols, int channels, int cvMattype,
			int frameLength);

		int streamType; //use BaseCam::StreamCapabilities enum to set this value
		int rows;
		int cols;
		int channels;
		int cvMatType;
		int frameLength;

		static std::vector<NetCamCVMatParams> Freenect2CVMatParams();

		const char *ToString();

		static void LoadFromString(NetCamCVMatParams &params, char *string);
	};

	///
	class Util {
	private:
		template<typename Out>
		static void Split(const std::string &s, char delim, Out result);

	public:
		static const boost::posix_time::ptime EPOCH;

		static size_t GetCVMatSize(cv::Mat mat);

		static std::vector<std::string> Split(const std::string &s, char delim);

		static void LoadFromStream(std::istream &stream);
	};
};


#endif //DYNCAM_TCPCOMMONS_H
