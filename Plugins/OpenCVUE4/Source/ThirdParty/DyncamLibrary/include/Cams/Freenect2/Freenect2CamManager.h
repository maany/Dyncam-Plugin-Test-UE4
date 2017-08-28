#pragma once

#include "Freenect2Cam.h"

namespace freenect2cam {

	/// Exposes functionality to manage multiple Freenect2Cam devices.
	/// @see Freenect2Cam documentation for a code example
	class Freenect2CamManager {
	private:
		// Used to communicate with libfreenect2
		libfreenect2::Freenect2 freenect2;

		// Devices that are currently connected and identified by this API
		std::vector<std::unique_ptr<Freenect2Cam>> connectedDevices;

		int numDevices; ///< number of devices detected during last call to FindDevices()
	public:
		/// The libfreenect2::Freenect2Device#open() function is executed by the constructor and a connection with the attached kinects.
		/// @return std::vector of Freenect2Cam pointers that are connected and identified by this API.
		vector<std::unique_ptr<Freenect2Cam>>& FindDevices();
	};
	
}
