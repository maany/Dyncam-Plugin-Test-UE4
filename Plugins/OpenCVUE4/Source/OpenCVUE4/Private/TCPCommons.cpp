//
// Created by mayank on 8/22/17.
//

#include "TCPCommons.h"

//**************************************************//
//          TCPCommons::NetCamCVMatParams           //
//**************************************************//
TCPCommons::NetCamCVMatParams::NetCamCVMatParams() {
    streamType = -1;
    rows = -1;
    cols = -1;
    channels = -1;
    cvMatType = -1;
    frameLength = -1;
}

TCPCommons::NetCamCVMatParams::NetCamCVMatParams(BaseCam::StreamCapabilities streamType, int rows, int cols,
                                                 int channels,
                                                 int cvMatType,
                                                 int frameLength) : streamType(static_cast<int>(streamType)),
                                                                    rows(rows),
                                                                    cols(cols), channels(channels),
                                                                    cvMatType(cvMatType),
                                                                    frameLength(frameLength) {

}

std::vector<TCPCommons::NetCamCVMatParams> TCPCommons::NetCamCVMatParams::Freenect2CVMatParams() {
    std::vector<NetCamCVMatParams> freenect2CamCVMatParams;
    NetCamCVMatParams freenectDepthCVMatParams(BaseCam::StreamCapabilities::DEPTH, 424, 512, 1, CV_32FC1, 868352);
    NetCamCVMatParams freenectColorCVMatParams(BaseCam::StreamCapabilities::COLOR, 1080, 1920, 4, CV_8UC4, 8294400);
    NetCamCVMatParams freenectIRCVMatParams(BaseCam::StreamCapabilities::IR, 424, 512, 1, CV_32FC1,
                                            868352); //TODO check and update the values
    freenect2CamCVMatParams.push_back(freenectDepthCVMatParams);
    freenect2CamCVMatParams.push_back(freenectColorCVMatParams);
    freenect2CamCVMatParams.push_back(freenectIRCVMatParams);
    return freenect2CamCVMatParams;
}

const char *TCPCommons::NetCamCVMatParams::ToString() {
    std::string netCamCVMatParams =
            std::to_string(streamType) + "," + std::to_string(rows) + "," + std::to_string(cols) + "," +
            std::to_string(channels) + "," + std::to_string(cvMatType) + "," + std::to_string(frameLength);
    const char *data = (const char *) malloc(sizeof(const char *) * netCamCVMatParams.length());
    memcpy((void *) data, netCamCVMatParams.data(), netCamCVMatParams.length());
    return data;
}

void TCPCommons::NetCamCVMatParams::LoadFromString(TCPCommons::NetCamCVMatParams &params, char *string) {
    std::vector<std::string> values = TCPCommons::Util::Split(string, ',');
    if (values.size() != 6)
        throw std::length_error(
                "[NetCamCVMatParams] : Cannot Load String as the data does not contain 6 csv values for representing streamType, rows, cols, channels, cvMatType, frameLength");
    params.streamType = atoi(values.at(0).c_str());
    params.rows = atoi(values.at(1).c_str());
    params.cols = atoi(values.at(2).c_str());
    params.channels = atoi(values.at(3).c_str());
    params.cvMatType = atoi(values.at(4).c_str());
    params.frameLength = atoi(values.at(5).c_str());
}

//**************************************************//
//              TCPCommons::Util                    //
//**************************************************//
const boost::posix_time::ptime TCPCommons::Util::EPOCH = boost::posix_time::ptime((boost::gregorian::date(2017, 1, 1)));

size_t TCPCommons::Util::GetCVMatSize(cv::Mat mat) {
    if (mat.isContinuous()) {
        return mat.total() * mat.elemSize();
    } else {
        return mat.step[0] * mat.rows;
    }
}

size_t GetCVMatSize(cv::Mat mat) {
    if (mat.isContinuous()) {
        return mat.total() * mat.elemSize();
    } else {
        return mat.step[0] * mat.rows;
    }
}

template<typename Out>
void TCPCommons::Util::Split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> TCPCommons::Util::Split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    Split(s, delim, std::back_inserter(elems));
    return elems;
}

void TCPCommons::Util::LoadFromStream(std::istream &stream) {
    std::string s;

    int p = stream.tellg();  // remember where we are

    stream.seekg(0, std::ios_base::end); // go to the end
    int np = stream.tellg();
    int sz = np - p;  // work out the size
    stream.seekg(p);        // restore the position

    s.resize(sz);          // resize the string
    stream.read(&s[0], sz);  // and finally, read in the data.
}



