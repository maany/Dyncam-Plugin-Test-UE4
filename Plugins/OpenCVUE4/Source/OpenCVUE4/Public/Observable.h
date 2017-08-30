#pragma once
#include "opencv2/opencv.hpp"

namespace dyncam_event {
	class Observer;
	class Observable {
		std::vector<Observer*> observers;
	public:
		void Attach(Observer* observer);
		void Emit(cv::Mat data);
	};
};
