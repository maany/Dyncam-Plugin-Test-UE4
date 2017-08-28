//
// Created by ascadian on 4/14/17.
//

#include "OpenNICam.h"
#include <chrono>

OpenNICam::OpenNICam() {
    openni::Status err;

    err = openni::OpenNI::initialize();
    if (err != openni::STATUS_OK)
    {
        throw std::runtime_error("Error at OpenNI2 initialization:\n"
                                 + std::string(openni::OpenNI::getExtendedError()));
    }

    openni::OpenNI::addDeviceConnectedListener(this);
    openni::OpenNI::addDeviceDisconnectedListener(this);
    openni::OpenNI::addDeviceStateChangedListener(this);

    openni::Array<openni::DeviceInfo> deviceList;
    openni::OpenNI::enumerateDevices(&deviceList);
    for (int i = 0; i < deviceList.getSize(); ++i)
    {
        printf("Device \"%s\" already connected\n", deviceList[i].getUri());
        m_device.open(deviceList[i].getUri());
        onDeviceOpened();
    }
}


OpenNICam::~OpenNICam() {
    m_doProcessing.store(false);
    m_streamThread.join();

    m_depthStream.stop();
    m_depthStream.destroy();
    m_device.close();
    openni::OpenNI::shutdown();
}


void OpenNICam::mythread() {
    using namespace std::chrono_literals;

    while(m_doProcessing.load())
    {
        updateFrame();
    }
}


bool OpenNICam::updateFrame() {
    openni::VideoFrameRef depthFrame;
    openni::Status err;

    int changedStreamDummy = -1; //The index of the first stream that has new frame available.
    openni::VideoStream* pStream = &m_depthStream;
    err = openni::OpenNI::waitForAnyStream(&pStream, 1, &changedStreamDummy, m_frameTimeout);
    if (err != openni::STATUS_OK)
    {
        printf("Wait failed! (timeout is %d ms)\n%s\n", m_frameTimeout, openni::OpenNI::getExtendedError());
        return false;
    }

    err = m_depthStream.readFrame(&depthFrame);
    if (err != openni::STATUS_OK) {
        std::cerr << "Could not read frame: " << std::string(openni::OpenNI::getExtendedError()) << std::endl;
        return false;
    }

    const auto *pDepthRow = (const openni::DepthPixel *) depthFrame.getData();

    // openni::DepthPixel -> uint_16
    auto width = depthFrame.getWidth();
    auto height = depthFrame.getHeight();
    cv::Mat img = cv::Mat(cv::Mat(height, width, CV_16UC1));
    //copy vector to mat
    memcpy(img.data, pDepthRow, width * height * sizeof(*pDepthRow));
    DepthEventsSource << img;

    return true;
}


void OpenNICam::onDeviceOpened() {
    openni::Status err;

    err = this->m_depthStream.create(this->m_device, openni::SENSOR_DEPTH);
    if (err != openni::STATUS_OK) {
        throw std::runtime_error("Creating depth stream failed:\n" + std::string(openni::OpenNI::getExtendedError()));
    }
    //std::cout << "mirroring: " << m_depthStream.getMirroringEnabled() << ", " << m_irStream.getMirroringEnabled() << std::endl;
    this->m_depthStream.setMirroringEnabled(false);

    // // set device resolution
    // const openni::SensorInfo* sinfo = this->m_device.getSensorInfo(openni::SENSOR_DEPTH);
    // const openni::Array< openni::VideoMode>& modesDepth = sinfo->getSupportedVideoModes();
    // std::cout << "Camera formats available: " << std::endl;
    // for (int i = 0; i < modesDepth.getSize(); ++i) {
    //      printf("%i: %ix%i, %i fps, %i format\n", i, modesDepth[i].getResolutionX(), modesDepth[i].getResolutionY(),
    //                          modesDepth[i].getFps(), modesDepth[i].getPixelFormat());
    // }
    // int format = 3;
    // printf("Using format: %ix%i, %i fps, %i\n", modesDepth[format].getResolutionX(), modesDepth[format].getResolutionY(),
    //                          modesDepth[format].getFps(), modesDepth[format].getPixelFormat());
    // err = this->m_depthStream.setVideoMode(modesDepth[format]);
    // if (err != openni::STATUS_OK) {
    //     throw std::runtime_error("Error setting video mode:\n" + std::string(openni::OpenNI::getExtendedError()));
    // }

    // start stream after setup
    err = this->m_depthStream.start();
    if (err != openni::STATUS_OK) {
        throw std::runtime_error("Starting depth stream failed:\n" + std::string(openni::OpenNI::getExtendedError()));
    }

    this->m_width = this->m_depthStream.getVideoMode().getResolutionX();
    this->m_height = this->m_depthStream.getVideoMode().getResolutionY();
    m_deviceConnected = true;

    // start listener thread
    m_doProcessing.store(true);
    m_streamThread = std::thread(&OpenNICam::mythread, this);
}


void OpenNICam::onDeviceConnected(const openni::DeviceInfo *device) {
    DeviceConnectListener::onDeviceConnected(device);

    if (m_deviceConnected) {
        std::cout << "Device ignored as we already are connected." << std::endl;
    } else {
        m_device.open(device->getUri());
    }   onDeviceOpened();
}


void OpenNICam::onDeviceDisconnected(const openni::DeviceInfo *device) {
    DeviceConnectListener::onDeviceDisconnected(device);
    m_device.close();
    m_deviceConnected = false;
}


void OpenNICam::onDeviceStateChanged(const openni::DeviceInfo *device, openni::DeviceState state) {
    DeviceConnectListener::onDeviceStateChanged(device, state);
}


bool OpenNICam::Open() {
    // we have to start streaming right when we open the device as structure is kinda broken
    return true;
}


void OpenNICam::Close() {
    // TODO
    BaseCam::Close();
}


std::set<BaseCam::StreamCapabilities> OpenNICam::getStreamCapabilities() {
    return {StreamCapabilities::IR, StreamCapabilities ::DEPTH};
}

