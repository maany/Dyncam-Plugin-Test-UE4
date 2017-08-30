#pragma once
#include "Observable.h"
#include "Observer.h"
using namespace dyncam_event;
void Observable::Attach(Observer* observer) {
	observers.push_back(observer);
}
void Observable::Emit(cv::Mat data) {
	for (int i = 0; i < observers.size();i++) {
		observers[i]->Notify(data);
	}
}
	
