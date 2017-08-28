#pragma once
#include <memory>
#include <opencv2/opencv.hpp>
namespace dyncam_unreal {
	class Event {
	private:
		struct EventP;
		std::unique_ptr<EventP> pImpl;
		Event();
	public:
		//BaseCam::StreamCapabilities m_StreamType;
		cv::Mat m_Mat;
		Event(cv::Mat mat);
	};
	class EventListener {
	public:
		virtual void OnReceive(Event event) = 0;
	};

	class DynCamUE4Wrapper {
	private:
		class DynCamUE4WrapperP;
		std::unique_ptr<DynCamUE4WrapperP> pImpl;
		//std::vector<EventListener> colorEventListeners;
		//std::vector<EventListener> depthEventListeners;
		//std::vector<EventListener> irEventListeners;
		void doConnect(char const * IPAddress, int port);
	public:
		//NetCam netcam;
		DynCamUE4Wrapper();
		//virtual void OnReceive(Event event);
		void Connect(char const * IPAddress, int port);
		void PublishCVMat(Event event);
		void SubscribeToColorEvents(EventListener* listener);
		void SubscribeToDepthEvents(EventListener* listener);
		void SubscribeToIREvents(EventListener* listener);


	};
}