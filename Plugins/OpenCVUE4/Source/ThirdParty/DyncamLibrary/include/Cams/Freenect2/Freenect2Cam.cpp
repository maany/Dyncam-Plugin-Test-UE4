//
// Created by maany on 6/26/17.
//

#include "Freenect2Cam.h"
#include "FreenectIntrinsics.h"
#include <libfreenect2/registration.h>

#define DEBUG_IDENTIFIER "<Freenect2Cam.h> : "

using namespace freenect2cam;

Freenect2Cam::Freenect2Cam(string serial, libfreenect2::PacketPipeline *pipeline) {
    this->serial = serial == "" ? freenect2.getDefaultDeviceSerialNumber() : serial;
    this->pipeline = pipeline;
}

bool Freenect2Cam::Open() {

    if (Connect()) {
        auto colorMask = libfreenect2::Frame::Color;
        auto depthMask = libfreenect2::Frame::Depth | libfreenect2::Frame::Ir;
        colorFrameListener = make_unique<libfreenect2::SyncMultiFrameListener>(colorMask);
        depthAndIRFrameListener = make_unique<libfreenect2::SyncMultiFrameListener>(depthMask);

        device->setColorFrameListener(colorFrameListener.get());
        device->setIrAndDepthFrameListener(depthAndIRFrameListener.get());

        m_resIR.Set( cv::Size( 512, 424 ) );
        m_resColor.Set( cv::Size( 1920, 1080 ) );



        // update our registration object on intrinsics change
        auto anyIntrinsicsChange = Monitor(IntrinsicsIRSignal) | Monitor(IntrinsicsColorSignal);
        Observe(anyIntrinsicsChange, With(IntrinsicsIRSignal, IntrinsicsColorSignal),
                [&device = device, &registration = registration](const cam_intrinsics_t &, const cam_intrinsics_t &intrIr,
                    const cam_intrinsics_t &intrColor)
                {
                    std::cout << "new registration created" << std::endl;
                    //            auto intrIR = intrinsicsBase2FreenectIR(intrIr);
                    //            auto intrCol = intrinsicsBase2FreenectColor(intrColor);
                    //            registration = std::make_shared<libfreenect2::Registration>(device->getIrCameraParams(),
                    //                                                                             device->getColorCameraParams());


                    //        registration = std::make_shared<libfreenect2::Registration>(device->getIrCameraParams(),
                    //                                                                    device->getColorCameraParams());
                    std::cout << "Creating registration\n";
                    registration = std::shared_ptr<libfreenect2::Registration>(
                            new libfreenect2::Registration(device->getIrCameraParams(),
                                                           device->getColorCameraParams()),
                            [=](libfreenect2::Registration *reg) {
                                std::cout << "Deleting registration\n";
                                delete reg;
                            });
                    if (!registration) {
                        std::cout << "allready null" << std::endl;
                    }
                });

        // make stream mode selectable at runtime
        Observe(streamMode, [this](StreamModes mode) {
            StopExistingStreams();
            if (mode == static_cast<StreamModes>(StreamMode::NO_STREAM) || !PreStreamChecks()) {
                return;
            }
            // all streams or specific
            if (mode == static_cast<StreamModes>(StreamMode::ALL)) {
                device->start();
            } else {
                bool color = mode & static_cast<StreamModes>(StreamMode::COLOR);
                bool depth = (mode & static_cast<StreamModes>(StreamMode::IR)) || (mode & static_cast<StreamModes>(StreamMode::DEPTH));
                device->startStreams(color, depth);
            }
            auto irparam = device->getIrCameraParams();
            registration = std::make_shared<libfreenect2::Registration>(irparam,
                                                                        device->getColorCameraParams());

            // start processing thread
            doProcessing.store(true);
            processingThread = thread(&Freenect2Cam::StartSuperStream, this, mode);
        });

        ConfigureStream(static_cast<freenect2cam::Freenect2Cam::StreamModes>(freenect2cam::Freenect2Cam::StreamMode::ALL));
        

        DoTransaction<D>([&m_intrinsicsIR = IntrinsicsIRSignal, &m_intrinsicsColor = IntrinsicsColorSignal, &device = device] {
            auto intrIR = intrinsicsFreenectIr2Base(device->getIrCameraParams());
            m_intrinsicsIR <<= intrIR;
            auto intrColor = intrinsicsFreenectColor2Base(device->getColorCameraParams());
            m_intrinsicsColor <<= intrColor;
        });
    }

    return state == State::DEVICE_OPEN;
}


std::set<BaseCam::StreamCapabilities> Freenect2Cam::getStreamCapabilities() {
    return {StreamCapabilities::COLOR
            ,StreamCapabilities::IR
            ,StreamCapabilities::DEPTH};
}


bool Freenect2Cam::PreStreamChecks() {
    // TODO: this will fail after the mode switch
    if (state != State::DEVICE_OPEN) {
        cerr << DEBUG_IDENTIFIER << "cannot find a connected Freenect2 device to stream data from" << endl;
        return false;
    }
    return true;
}

bool Freenect2Cam::Connect() {
    if (pipeline != nullptr) {
        device = freenect2.openDevice(serial, pipeline);
    } else {
        device = freenect2.openDevice(serial);
    }

    if (device == nullptr) {
        std::cout << DEBUG_IDENTIFIER << "failure opening device!" << std::endl;
        state = State::DEVICE_FAILED;
    } else {
        state = State::DEVICE_OPEN;
    }

    return State::DEVICE_OPEN == state;
}

bool Freenect2Cam::Disconnect() {
    if (state != State::DEVICE_OPEN) {
        cerr << DEBUG_IDENTIFIER << "please make sure a Freenect2 device is connected before trying to disconnect"
             << endl;
        return false;
    }
    device->stop();
    device->close();
    state = State::DEVICE_CLOSED;
    return true;
}

void Freenect2Cam::ConfigureStream(StreamModes streamMode) {
    this->streamMode.Set(streamMode);
}

bool Freenect2Cam::StopExistingStreams() {
    // stop thread
    if (doProcessing.load()) {
        doProcessing.store(false);
        processingThread.join();
    }

    // device will be restarted depending on the stream mode
    if (device != nullptr) {
        return device->stop();
    }
    return true;
}


bool Freenect2Cam::StartSuperStream(const StreamModes streamModes) {

    libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4);

    while(doProcessing.load()){
        bool process_color = streamModes & static_cast<StreamModes>(StreamMode::COLOR);
        bool process_depth = (streamModes & static_cast<StreamModes>(StreamMode::IR)) || (streamModes & static_cast<StreamModes>(StreamMode::DEPTH));
        if (process_depth) {
            if (!depthAndIRFrameListener->waitForNewFrame(depthAndIrFrames,
                                                          waitFrameTimeout)) {
                std::cout << "timeout!" << std::endl;
                return false;
            }

            if (streamModes & static_cast<StreamModes>(StreamMode::DEPTH)) {
                libfreenect2::Frame *depth = depthAndIrFrames[libfreenect2::Frame::Depth];
                cv::Mat depthMat(static_cast<int>(depth->height), static_cast<int>(depth->width),
                                 CV_32FC1, depth->data);
                DepthEventsSource << depthMat;
            }

            if (streamModes & static_cast<StreamModes>(StreamMode::IR)) {
                libfreenect2::Frame *ir = depthAndIrFrames[libfreenect2::Frame::Ir];
                cv::Mat irMat(static_cast<int>(ir->height), static_cast<int>(ir->width),
                              CV_32FC1, ir->data);
                IrEventsSource << irMat;
            }

        }

        if (process_color) {
            if (!colorFrameListener->waitForNewFrame(colorFrames, waitFrameTimeout)) {
                std::cout << "timeout!" << std::endl;
                return false;
            }
            libfreenect2::Frame *color = colorFrames[libfreenect2::Frame::Color];
            cv::Mat colorMat(static_cast<int>(color->height), static_cast<int>(color->width),
                             CV_8UC4, color->data);
            ColorEventsSource << colorMat;
        }


        // get point cloud
        if (process_color && process_depth) {
            // TODO: this mutex could be released after the mat creation
            std::lock_guard<std::mutex> lock(reconstructMutex);
            registration->apply(colorFrames[libfreenect2::Frame::Color],
                                depthAndIrFrames[libfreenect2::Frame::Depth],
                                &undistorted, &registered);

            cv::Mat registeredColorMat(static_cast<int>(registered.height),
                                       static_cast<int>(registered.width),
                                       CV_8UC4, registered.data);
            cv::Mat undistortedDepthMat(static_cast<int>(undistorted.height),
                                        static_cast<int>(undistorted.width),
                                        CV_8UC4, undistorted.data);

            RegisteredColorEventsSource << registeredColorMat;
            UndistortedDepthEventsSource << undistortedDepthMat;

        } else if (process_depth){
            // TODO: this mutex could be released after the mat creation
            std::lock_guard<std::mutex> lock(reconstructMutex);
            registration->undistortDepth(depthAndIrFrames[libfreenect2::Frame::Depth], &undistorted);

            cv::Mat undistortedDepthMat(static_cast<int>(undistorted.height),
                                        static_cast<int>(undistorted.width),
                                        CV_8UC4, undistorted.data);

            UndistortedDepthEventsSource << undistortedDepthMat;

        }

        // free here as we need the frames for the cloud processing
        if (process_color) {
            colorFrameListener->release(colorFrames);
        }
        if (process_depth) {
            depthAndIRFrameListener->release(depthAndIrFrames);
        }
    }
    return true;
}

Freenect2Cam::~Freenect2Cam(){
    StopExistingStreams();
    // I guess we have to call some disconnection method here
    Disconnect();
}

