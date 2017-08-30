#pragma once
#include "Observer.h"
using namespace dyncam_event;

void Observer::Notify(cv::Mat data) {
	m_callback(data);
}
void Observer::Observe(Observable* observable, std::function<void(cv::Mat)> callback) {
	observable->Attach(this);
	m_callback = callback;
}

