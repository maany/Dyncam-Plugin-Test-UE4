#include <opencv2/opencv.hpp>

#include <chrono>
#include <functional>
#include <memory>
#include <random>
#include <thread>
#include "Cams/Freenect2/Freenect2Cam.h"
#include "Cams/Freenect2/Freenect2CamManager.h"
#include "Cams/OpenCVCam/OpenCVCam.h"
#include "Tracker/PatternTracker.h"

#include "ReactDomain.h"

int main()
{
    // create, open and track the pattern in camera 1
    auto cam1 = std::make_shared<freenect2cam::Freenect2Cam>( "093487434347", new libfreenect2::OpenCLPacketPipeline() );
    if( !cam1->Open() )
        return EXIT_FAILURE;

    auto patternTracker1 = std::make_unique<PatternTracker>( cam1, cv::Size(3,9), 0.02 );
    auto camTransform1 = patternTracker1->start();


    // create, open and track the pattern in camera 2
    /*auto cam2 = std::make_shared<freenect2cam::Freenect2Cam>( "001061662047", new libfreenect2::OpenCLPacketPipeline() );
    cam2->Open();

    auto patternTracker2 = std::make_unique<PatternTracker>( cam2, cv::Size(3,9), 0.02 );
    auto camTransform2 = patternTracker2->start();*/


    //auto anyTrans = camTransform1 | camTransform2;

    Observe( camTransform1, [](cv::Matx44d mat) {
        if( mat != cv::Matx44d::eye() )
            std::cout << "Mat: " << mat << std::endl;
    });


    bool stop = false;
    while (!stop)
        stop = (cv::waitKey(10) == -1);

    return EXIT_SUCCESS;
}
