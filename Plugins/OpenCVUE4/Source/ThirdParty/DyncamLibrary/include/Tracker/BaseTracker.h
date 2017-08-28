//
// Created by ascadian on 7/26/17.
//

#ifndef DYNCAM_BASETRACKER_H
#define DYNCAM_BASETRACKER_H

//#include <Eigen/Dense>

#include <tuple>

#include "../ReactDomain.h"

#include <opencv2/opencv.hpp>

class BaseTracker {

public:
	enum class Handedness : unsigned int {
		RIGHTHANDED, LEFTHANDED
	};

	Handedness m_handedness = Handedness::RIGHTHANDED;
	std::string m_name = "";

	virtual EventsT<cv::Matx44d> start() { return MakeEventSource<D, cv::Matx44d>(); }
};


#endif //DYNCAM_BASETRACKER_H
