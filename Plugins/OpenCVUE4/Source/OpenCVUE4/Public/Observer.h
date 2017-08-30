#pragma once
#include "opencv2/opencv.hpp"
#include "Observable.h"
namespace dyncam_event {
	
	class Observer {
		std::function<void(cv::Mat)> m_callback;
	public:
		void Notify(cv::Mat data);
		void Observe(Observable* observable, std::function<void(cv::Mat)> callback);
	};
};