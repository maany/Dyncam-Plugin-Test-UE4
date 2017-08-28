/// @file
/// @author     Mayank Sharma
/// @version    ...
///
/// @section LICENSE
///
/// ...
///
/// @section DESCRIPTION
///
/// This file declares
/// <ul>
/// <li> Freenect2Cam : Class representing a Freenect2 camera.</li>
/// </ul>
/// It exposes the data streams from the camera through Reactive Event Streams
/// described by CppReact.

#ifndef DYNCAM_FREENECT2CAM_H
#define DYNCAM_FREENECT2CAM_H

#include "../../ReactDomain.h"

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h> //optional
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h> //optional

#include <opencv2/opencv.hpp>
#include <vector>
#include <mutex>

#include "../BaseCam.h"

/// exposes freenect2 support for dyncam
namespace freenect2cam {

    using namespace std;
    using namespace react;
    using namespace cv;

	

	/// Class representing a Freenect2 Camera. It wraps around freenect2::Freenect2 camera to make it reactive, multithreaded.
	/// It also provides better control over when to connect, disconnect and what type to data to stream
	/// Usage :
	/// \code
	///    void freenectLauncher(){
	///        freenect2cam::Freenect2CamManager freenect2CamManager; // Freenect2CamManager is needed to create Freenect2Cam objects for connected kinect devices
	///        std::vector<freenect2cam::Freenect2Cam*> freenectVector = freenect2CamManager.FindDevices(); // the call to FindDevices creates Freenect2Cam objects for connected devices
	///        // loop to traverse through the connected devices
	///         for(int i=0;i<freenectVector.size();i++){
	///            freenect2cam::Freenect2Cam* freenect2Cam = freenectVector.at(i);
	///            std::cout<<freenect2Cam->serial<<std::endl;
	///            // Set up your listeners!! i.e. Register reactive observers or listeners that receive the cv::Mat created for Depth, IR and Color Frames streamed for the current Freenect2Cam
	///            Observe(freenect2Cam->depthEventStream,[] (cv::Mat depthMat){
	///                cv::imshow("depthWin", depthMat);
	///                cv::waitKey(1);
	///            });
	///            Observe(freenect2Cam->colorEventStream,[] (cv::Mat colorMat){
	///                cv::imshow("rgbWin", colorMat);
	///                cv::waitKey(1);
	///            });
	///            Observe(freenect2Cam->irEventStream,[] (cv::Mat irMat){
	///                cv::imshow("irWin",irMat);
	///                cv::waitKey(1);
	///            });
	///             // Set up What you wanna listen to i.e. ConfigureStream will set StreamMode and determine which of the observers set in the lines of code above will get triggered
	///            freenect2Cam->ConfigureStream(freenect2cam::StreamMode::DEPTH_ONLY);
	///        }
	///    }
	/// \endcode
    class Freenect2Cam : public BaseCam {
        friend class FreenectCloudMapper;

	public:

		/// holds an union of stream modes
		using StreamModes = unsigned int;

		/// represents the reactive event streams that you can/want to listen to
		enum class StreamMode : unsigned int {
			NO_STREAM = 0,
			COLOR = 1,
			IR = 2,
			DEPTH = 4,

			ALL = COLOR | IR | DEPTH
		};


	protected:

		// Maximum time we wait for the next frame
        static const int waitFrameTimeout = 2*1000; // 2 sconds

		// Object used to manage communications with the Kinect
        libfreenect2::Freenect2 freenect2;

        // TODO: make uniqe ptr with custom deleter
        // Device used to communicate with Kinect
        libfreenect2::Freenect2Device *device = nullptr;

        std::shared_ptr<libfreenect2::Registration> registration;

        // TODO: make shared ptr with custom deleter
		// CPU/GPU pipeline
        libfreenect2::PacketPipeline *pipeline = nullptr;

        unique_ptr<libfreenect2::SyncMultiFrameListener> depthAndIRFrameListener;
        unique_ptr<libfreenect2::SyncMultiFrameListener> colorFrameListener;

        libfreenect2::FrameMap colorFrames;
        libfreenect2::FrameMap depthAndIrFrames;

		// Stops all streaming and cam reading threads
        bool StopExistingStreams();
		
		// Start streaming data and populating depthEventStream, irEventStream and colorEventStream
        bool StartSuperStream(const StreamModes streamModes);

		// Checks State to determine whether to stream data or not
        bool PreStreamChecks();

		// Thread used to read data from camera
        thread processingThread;
		
		// If true, superThread will continue execution. Will stop executing otherwise.
        atomic<bool> doProcessing{false};

		// during point cloud reconstruction we can't rewrite the buffer
		std::mutex reconstructMutex;

		// open and connects to libfreenect2::Freenect2Device
        bool Connect();
        bool Disconnect(); ///<stop streaming and close libfreenect2::Freenect2Device
		
    public:
		// Serial no for this Freenect2Cam
        std::string serial = "";
		virtual std::string name() override { return serial; }

        void ConfigureStream(StreamModes streamMode); ///< set up the StreamMode which will determine the reactive streams (depthEventStream, colorEventStream, irEventStream) that will be populated with cv::Mat data.

        virtual std::set<BaseCam::StreamCapabilities> getStreamCapabilities() override;
		bool Open() override;

        /// @param serial no of the libfreenect2::Freenect2 device this class wraps around
        /// @param pipeline libfreenect2::PacketPipeline to use when connecting to libfreenect2::Freenect2
        Freenect2Cam(string serial = "", libfreenect2::PacketPipeline *pipeline = new libfreenect2::OpenGLPacketPipeline());

        ~Freenect2Cam();




		// Curretn mode. We listen to changes and restart the streams accordingly
		D::VarSignalT<StreamModes> streamMode = MakeVar<D>(static_cast<StreamModes>(StreamMode::NO_STREAM));

    };

    
}

#endif //DYNCAM_FREENECT2CAM_H

