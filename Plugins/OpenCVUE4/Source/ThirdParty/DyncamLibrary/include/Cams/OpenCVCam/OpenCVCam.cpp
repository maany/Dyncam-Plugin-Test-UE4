//
// Created by ascadian on 7/28/17.
//

#include "OpenCVCam.h"

//#include "opencv2/opencv.hpp"

using namespace cv;



OpenCVCam::OpenCVCam() {
}

OpenCVCam::~OpenCVCam() {
    Close();
}

std::set<BaseCam::StreamCapabilities> OpenCVCam::getStreamCapabilities() {
    return {StreamCapabilities::COLOR};
}

bool OpenCVCam::Open() {
    BaseCam::Open();

    m_device = std::make_unique<VideoCapture>(0); // open the default camera
    if(!m_device->isOpened()) {
        state = State::DEVICE_FAILED;
        return false;
    }

    m_doProcessing.store(true);
    m_streamThread = std::thread(&OpenCVCam::streamThread, this);
    state = State::DEVICE_OPEN;
    return true;
}

void OpenCVCam::Close() {
    BaseCam::Close();

    m_doProcessing.store(false);
    m_streamThread.join();

    state = State::DEVICE_CLOSED;
}

void OpenCVCam::streamThread() {
    while(m_doProcessing.load())
    {
        Mat frame;
        if (m_device->read(frame)) {
            //std::cout << "read" << std::endl;
            ColorEventsSource << frame;
        }
    }
}

