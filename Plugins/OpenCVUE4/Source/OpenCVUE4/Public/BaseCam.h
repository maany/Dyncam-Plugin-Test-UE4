#pragma once
#include "opencv2/opencv.hpp"
#include "Observable.h"
using namespace dyncam_event;
class BaseCam {
public:
	/// represents the current state of the camera driver
	enum class State {
		DEVICE_UNINITIALIZED, ///< No connection or data streams have been set up
		DEVICE_FAILED, ///< Opening device failed
		DEVICE_OPEN, ///< Connection established with Kinectv2 sensor
		DEVICE_CLOSED ///< Freenect2Cam is closed. To restart, use Freenect2Cam::Connect()
	};

	enum class StreamCapabilities : unsigned int {
		COLOR, IR, DEPTH
	};
	Observable DepthEventsSource;
	Observable ColorEventsSource;
	Observable IrEventsSource;
	Observable PointCloudEventsSource;
	// current State of the camera
	//State state = State::DEVICE_UNINITIALIZED;

	//virtual bool Open() { return false; };
	//virtual void Close() {};

	//virtual std::string name() { return ""; }

	//virtual std::set<StreamCapabilities> getStreamCapabilities() = 0;

	//D::VarSignalT <State> m_state = MakeVar<D>(State::DEVICE_UNINITIALIZED);
	//D::VarSignalT <cv::Size> m_resIR = MakeVar<D>(cv::Size(0u, 0u));
	//D::VarSignalT <cv::Size> m_resColor = MakeVar<D>(cv::Size(0u, 0u));


	//cam_intrinsics_t nullIntrinsics;
	//D::VarSignalT <cam_intrinsics_t> IntrinsicsIRSignal = MakeVar<D>(nullIntrinsics);
	//D::VarSignalT <cam_intrinsics_t> IntrinsicsColorSignal = MakeVar<D>(nullIntrinsics);

	//EventSourceT<cv::Mat> DepthEventsSource = MakeEventSource<D, cv::Mat>();
	//EventSourceT<cv::Mat> ColorEventsSource = MakeEventSource<D, cv::Mat>();
	//EventSourceT<cv::Mat> IrEventsSource = MakeEventSource<D, cv::Mat>();

	//EventSourceT<cv::Mat> UndistortedDepthEventsSource = MakeEventSource<D, cv::Mat>();
	//EventSourceT<cv::Mat> RegisteredColorEventsSource = MakeEventSource<D, cv::Mat>();
};