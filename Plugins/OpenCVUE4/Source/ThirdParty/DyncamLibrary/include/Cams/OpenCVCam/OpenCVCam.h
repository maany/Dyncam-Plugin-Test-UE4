//
// Created by ascadian on 7/28/17.
//

#ifndef DYNCAM_OPENCVCAM_H
#define DYNCAM_OPENCVCAM_H

#include "../BaseCam.h"

class OpenCVCam : public BaseCam {
public:
    OpenCVCam();
    ~OpenCVCam();

    virtual bool Open() override;
    virtual void Close() override;

    virtual std::set<BaseCam::StreamCapabilities> getStreamCapabilities() override;

protected:
    std::thread m_streamThread;
    void streamThread();

    // If true, superThread will continue execution. Will stop executing otherwise.
    std::atomic<bool> m_doProcessing{false};

    std::unique_ptr<cv::VideoCapture> m_device;
};


#endif //DYNCAM_OPENCVCAM_H
