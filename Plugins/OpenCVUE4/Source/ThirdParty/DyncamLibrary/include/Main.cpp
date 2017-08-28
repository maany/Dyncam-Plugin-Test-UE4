#include <opencv2/opencv.hpp>

#include <chrono>
#include <functional>
#include <memory>
#include <random>
#include <thread>
#include "Cams/Freenect2/Freenect2Cam.h"
#include "Cams/Freenect2/Freenect2CamManager.h"
#include "Cams/NetCam/NetCamServer.h"
#include "Cams/NetCam/NetCam.h"
//#include "Cams/OpenCVCam/OpenCVCam.h"

#include "ReactDomain.h"


void freenectTest(){
    std::cout << "Testing C++React with freenect2\n\n";

    freenect2cam::Freenect2CamManager freenect2CamManager;
    auto& freenectVector = freenect2CamManager.FindDevices();
    for (auto const& freenect2Cam : freenectVector) {
        std::cout << freenect2Cam->serial << std::endl;
        Observe(freenect2Cam->DepthEventsSource,[] (cv::Mat depthMat){
            cv::imshow("depthWin", depthMat);
            cv::waitKey(1);
        });

        Observe(freenect2Cam->ColorEventsSource,[] (cv::Mat colorMat){
            cv::imshow("rgbWin", colorMat);
            cv::waitKey(1);
        });
        Observe(freenect2Cam->IrEventsSource,[] (cv::Mat irMat){
            cv::imshow("irWin",irMat);
            cv::waitKey(1);
        });
		
        freenect2Cam->ConfigureStream(static_cast<freenect2cam::Freenect2Cam::StreamModes>(freenect2cam::Freenect2Cam::StreamMode::ALL));
    }
	bool stop = false;
    while (!stop)
        stop = (cv::waitKey(10) == -1);
}

/*void cvCamTest() {
    OpenCVCam cam{};
    if (cam.Open()) {
        std::cout << "Open :)" << std::endl;
    }
    cv::namedWindow("Color", CV_WINDOW_NORMAL);
    Observe(cam.ColorEventsSource, [] (cv::Mat someImage) {
        cv::imshow("Color", someImage);
    });
    EventsT<cv::Mat> edge = Transform(cam.ColorEventsSource, [] (cv::Mat image) {
        cv::Mat edges;
        cvtColor(image, edges, CV_BGR2GRAY);
        GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);
        return edges;
    });
    cv::namedWindow("Edges", CV_WINDOW_NORMAL);
    Observe(edge, [] (cv::Mat someImage) {
        cv::imshow("Ed	bool stop = false;
    while (!stop)
        stop = (cv::waitKey(10) == -1);ges", someImage);
    });
}*/
void netcamClientLauncher() {
    NetCam client;
    Observe(client.DepthEventsSource, [](cv::Mat mat){
        cv::imshow("Depth on NetCam", mat);
        cv::waitKey(1);
    });
    Observe(client.ColorEventsSource, [](cv::Mat mat){
        cv::imshow("Color on NetCam", mat);
        cv::waitKey(1);
    });
    std::vector<BaseCam::StreamCapabilities> requestStreams;
    requestStreams.push_back(BaseCam::StreamCapabilities::DEPTH);
    requestStreams.push_back(BaseCam::StreamCapabilities::COLOR);
    client.Connect("127.0.0.1", 31401, requestStreams);
}
void doStartServer(NetCamServer* server){
    server->StartServer();
}
BaseCam::StreamCapabilities identifyFreenectStreamType(cv::Mat& mat){
    if(mat.rows==424){
        return BaseCam::StreamCapabilities ::DEPTH;
    }else if(mat.rows ==1080){
        return  BaseCam::StreamCapabilities ::COLOR;
    }else {
        return BaseCam::StreamCapabilities ::IR;
    }
}
void netcamServerLauncher() {
//    freenect2cam::Freenect2Cam cam{};
//    NetCamServer server(TCPCommons::Protocol::IPv4, 31401, &cam,TCPCommons::NetCamCVMatParams::Freenect2CVMatParams(),identifyFreenectStreamType);
//    std::thread serverThread(doStartServer,&server);
//    serverThread.join();
//    cam.Open();
//    std::cout<<"[netCamServerLauncher] : Thread Complete"<<std::endl;
//
//    // we need to keep the camera alive here
//    bool stop = false;
//    while (!stop)
//        stop = (cv::waitKey(10) == -1);

    freenect2cam::Freenect2CamManager freenect2CamManager;
    auto& freenectVector = freenect2CamManager.FindDevices();
    for (auto const& freenect2Cam : freenectVector) {
        NetCamServer server(TCPCommons::Protocol::IPv4, 31401, freenect2Cam.get(),TCPCommons::NetCamCVMatParams::Freenect2CVMatParams(),identifyFreenectStreamType);
        std::thread serverThread(doStartServer,&server);
        freenect2Cam->Open();
        bool stop = false;
        while (true)
            stop = (cv::waitKey(10) == -1);
        serverThread.join();
        std::cout<<"[netCamServerLauncher] : Thread Complete"<<std::endl;
    }
}

void netcamLauncher(int i) {
    if(i==0) {
        std::thread(netcamServerLauncher).join();

    }else if (i==1) {
        std::thread clientThread(netcamClientLauncher);
        clientThread.join();
    }
}

int main(int argc,char *argv[])
{
    std::cout << "Testing C++React with NetCamServer\n\n";
    netcamLauncher(atoi(argv[1]));
    std::cout << "Exiting \n\n";

    //cvCamTest();
//    freenectTest();
//	freenect2cam::Freenect2CamManager freenect2CamManager;
//  freenect2cam::Freenect2Cam* cam = freenect2CamManager.FindDevices().front().get();
//  cam->ConfigureStream(static_cast<freenect2cam::StreamModes>(freenect2cam::StreamMode::ALL));

/*
    //auto cam = std::make_unique<OpenNICam>();
    auto cam = std::make_unique<freenect2cam::Freenect2Cam>();
    cam->Open();


    EventsT<cv::Mat> stretched = Transform(cam->DepthEventsSource, [](cv::Mat image) {
        // Histogram Equalization
        double min;
        double max;
        cv::minMaxIdx(image, &min, &max);
        cv::Mat adjMap = cv::Mat();
        // expand your range to 0..255. Similar to histEq();
        //(*image).convertTo(adjMap, CV_8UC1, 255 / (max-min), -255*min/(max-min)); // try this if the other method fails
        image.convertTo(adjMap, CV_8UC1, 255 / (max - min), -min);
        return adjMap;
    });

    EventsT<cv::Mat> colored = Transform(stretched, [](cv::Mat image) {
        // Convert grey scaled image to tone mapped
        cv::Mat falseColorsMap = cv::Mat();
        applyColorMap(image, falseColorsMap, cv::COLORMAP_AUTUMN);
        return falseColorsMap;
    });

    cv::namedWindow("Depth", CV_WINDOW_NORMAL);
    Observe(stretched, [](cv::Mat someImage) {
        cv::imshow("Depth", someImage);
    });

    cv::namedWindow("Colored depth", CV_WINDOW_NORMAL);
    Observe(colored, [](cv::Mat someImage) {
        cv::imshow("Colored depth", someImage);
    });

    auto capabilities = cam->getStreamCapabilities();
    if (capabilities.find(BaseCam::StreamCapabilities::COLOR) != capabilities.end()) {
        cv::namedWindow("Color", CV_WINDOW_NORMAL);
        Observe(cam->ColorEventsSource, [](cv::Mat someImage) {
            cv::imshow("Color", someImage);
        });
    }


    bool stop = false;
    while (!stop)
        stop = (cv::waitKey(10) == -1);*/

    return EXIT_SUCCESS;
}
