//
// Created by ascadian on 4/14/17.
//

#ifndef DYNCAM_OPENNICAM_H
#define DYNCAM_OPENNICAM_H


#include <react/Domain.h>
#include <react/Event.h>
#include <react/Observer.h>
#include <opencv2/core/mat.hpp>

#include "OpenNI.h"

#include "../../ReactDomain.h"
#include "../BaseCam.h"

class DeviceConnectListener : public openni::OpenNI::DeviceConnectedListener
        , public openni::OpenNI::DeviceDisconnectedListener, public openni::OpenNI::DeviceStateChangedListener
{
public:

    virtual void onDeviceConnected( const openni::DeviceInfo* device )
    {
        printf("onDeviceConnected : %s \n", device->getName());
    }

    virtual void onDeviceDisconnected( const openni::DeviceInfo* device )
    {
        printf("onDeviceDisconnected : %s \n", device->getName());
    }

    virtual void onDeviceStateChanged(const openni::DeviceInfo* device, openni::DeviceState state)
    {
        printf("onDeviceChanged : %s code: %d  \n", device->getName(), state);
    }
};

class OpenNICam  : public DeviceConnectListener, public BaseCam{
public:
    OpenNICam();
    virtual ~OpenNICam();

    virtual bool Open() override;
    virtual void Close() override;

    virtual std::set<BaseCam::StreamCapabilities> getStreamCapabilities() override;

protected:
    void onDeviceOpened();

    virtual void onDeviceConnected( const openni::DeviceInfo* device ) override;

    virtual void onDeviceDisconnected( const openni::DeviceInfo* device ) override;

    virtual void onDeviceStateChanged(const openni::DeviceInfo* device, openni::DeviceState state) override;


    openni::Device m_device;
    openni::VideoStream m_depthStream;
    openni::VideoStream m_irStream;

    int m_width = 0;
    int m_height = 0;

    // If true, superThread will continue execution. Will stop executing otherwise.
    std::atomic<bool> m_doProcessing{false};

    bool m_deviceConnected = false;

    std::thread m_streamThread;
    void mythread();

    bool updateFrame();

    const int m_frameTimeout = 2000; //2000ms
};


#endif //DYNCAM_OPENNICAM_H
