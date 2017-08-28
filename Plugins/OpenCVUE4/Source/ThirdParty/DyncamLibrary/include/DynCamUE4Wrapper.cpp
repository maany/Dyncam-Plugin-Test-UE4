#include "DynCamUE4Wrapper.h"
#include "Cams\NetCam\NetCam.h"
#include "Cams\BaseCam.h"
#include "ReactDomain.h"

using namespace dyncam_unreal;
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
	return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
struct Event::EventP {
	BaseCam::StreamCapabilities m_StreamType;
};

struct DynCamUE4Wrapper::DynCamUE4WrapperP {
	std::vector<EventListener> colorEventListeners;
	std::vector<EventListener> depthEventListeners;
	std::vector<EventListener> irEventListeners;
	NetCam netcam;
};
DynCamUE4Wrapper::DynCamUE4Wrapper() : pImpl(make_unique<DynCamUE4WrapperP>()) {
	
}
Event::Event(cv::Mat mat) : pImpl(std::make_unique<EventP>()) {
	m_Mat = mat;
}
void DynCamUE4Wrapper::PublishCVMat(Event event) {
	/*switch (event.m_StreamType) {
	case  BaseCam::StreamCapabilities::DEPTH :
		for (int i = 0; i < depthEventListeners.size(); i++) {
			depthEventListeners[i]->OnReceive(event);
		}
		break;
	case BaseCam::StreamCapabilities::COLOR :
		for (int i = 0; i < colorEventListeners.size(); i++) {
			colorEventListeners[i]->OnReceive(event);
		}
		break;
	case BaseCam::StreamCapabilities::IR :
		for (int i = 0; i < irEventListeners.size(); i++) {
			irEventListeners[i]->OnReceive(event);
		}
		break;
	}*/
}
//void DynCamUE4Wrapper::SubscribeToColorEvents(EventListener* listener) {
//	colorEventListeners.push_back(listener);
//}
//void DynCamUE4Wrapper::SubscribeToDepthEvents(EventListener* listener) {
//	depthEventListeners.push_back(listener);
//}
//void DynCamUE4Wrapper::SubscribeToIREvents(EventListener* listener) {
//	irEventListeners.push_back(listener);
//}
//void DynCamUE4Wrapper::Connect(char const * IPAddress, int port, std::vector<BaseCam::StreamCapabilities> requestStreams) {
//	std::thread(&DynCamUE4Wrapper::doConnect, this, IPAddress, port, requestStreams);
//}
//void DynCamUE4Wrapper::doConnect(char const * IPAddress, int port, std::vector<BaseCam::StreamCapabilities> requestStreams) {
//	Observe(netcam.DepthEventsSource, [&](cv::Mat mat) {
//		PublishCVMat(Event(BaseCam::StreamCapabilities::DEPTH, mat));
//	});
//	Observe(netcam.ColorEventsSource, [&](cv::Mat mat) {
//		PublishCVMat(Event(BaseCam::StreamCapabilities::COLOR, mat));
//	});
//	Observe(netcam.IrEventsSource, [&](cv::Mat mat) {
//		PublishCVMat(Event(BaseCam::StreamCapabilities::IR, mat));
//	});
//	netcam.Connect(IPAddress, port, requestStreams);
//}