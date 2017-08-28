
#include "Freenect2CamManager.h"

using namespace freenect2cam;

vector<std::unique_ptr<Freenect2Cam>>& Freenect2CamManager::FindDevices() {
    libfreenect2::setGlobalLogger(libfreenect2::createConsoleLogger(libfreenect2::Logger::Debug));

    numDevices = freenect2.enumerateDevices();
    //connectedDevices = vector<Freenect2Cam *>();
    for (int i = 0; i < numDevices; i++) {
        string serial = freenect2.getDeviceSerialNumber(i);
        connectedDevices.push_back(make_unique<Freenect2Cam>(serial));
    }
    return connectedDevices;
}
