////
//// Created by devmaany on 08.08.17.
////
//
//#ifndef DYNCAM_TCPCONNECTION_H
//#define DYNCAM_TCPCONNECTION_H
//
//#define MAX_NO_OF_SUPPORTED_STREAM_TYPES 3
//
//#include "chrono"
////include boost
//#include "boost/asio.hpp"
//#include "boost/array.hpp"
//#include <boost/enable_shared_from_this.hpp>
//#include <boost/shared_ptr.hpp>
//#include "boost/bind.hpp"
//#include "boost/asio/steady_timer.hpp"
//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/json_parser.hpp>
//#include <boost/functional/hash.hpp>
//#include "../BaseCam.h"
//// include OpenCV
//#include <opencv2/opencv.hpp>
//#include "../BaseCam.h"
//using namespace boost::asio::ip;
//using namespace std;
//
//class NetCamCVMatParams{
//public:
//    NetCamCVMatParams();
//
//    NetCamCVMatParams(BaseCam::StreamCapabilities streamType, int rows, int cols, int channels, int cvMattype,
//                      int frameLength);
//
//    int streamType; //use BaseCam::StreamCapabilities enum to set this value
//    int rows;
//    int cols;
//    int channels;
//    int cvMatType;
//    int frameLength;
//    static vector<NetCamCVMatParams> Freenect2CVMatParams();
//    const char* ToString();
//    static void LoadFromString(NetCamCVMatParams& params,char* string);
//};
//
//enum ServerProtocol {
//    IPv4, IPv6
//};
//
//class FramePacket{
//    static FramePacket assembledFramePacket;
//    long startTime;
//    long endTime;
//public:
//    string dataType;
//    string data;
//    void LoadAsString(const string message);
//    string saveAsString();
//    void SaveFramePacket(std::ostream &request_stream);
//    static FramePacket* AssemblePacket(std::istream& response_stream);
//
//};
//
//class Util {
//public:
//    static size_t GetCVMatSize(cv::Mat mat){
//        if (mat.isContinuous()) {
//            return mat.total() * mat.elemSize();
//        } else {
//            return mat.step[0] * mat.rows;
//        }
//    }
//
//    template<typename Out>
//    static void split(const std::string &s, char delim, Out result) {
//        std::stringstream ss;
//        ss.str(s);
//        std::string item;
//        while (std::getline(ss, item, delim)) {
//            *(result++) = item;
//        }
//    }
//    static std::vector<std::string> split(const std::string &s, char delim) {
//        std::vector<std::string> elems;
//        split(s, delim, std::back_inserter(elems));
//        return elems;
//    }
//    static void LoadFromStream(std::istream & stream) {
//        std::string s;
//
//        int p = stream.tellg();  // remember where we are
//
//        stream.seekg(0, std::ios_base::end); // go to the end
//        int np = stream.tellg();
//        int sz = np - p;  // work out the size
//        stream.seekg(p);        // restore the position
//
//        s.resize(sz);          // resize the string
//        stream.read(&s[0], sz);  // and finally, read in the data.
//    }
//    static uint8_t* MatToBytes(cv::Mat image)
//    {
//        int size = GetCVMatSize(image);
//        uint8_t* bytes = new uint8_t[size];  // delete[] that later
//        std::memcpy(bytes,image.data,size * sizeof(uint8_t));
//        return bytes;
//    }
//    static cv::Mat BytesToMat(uint8_t* bytes, int height, int width, BaseCam::StreamCapabilities type){
//        if(type == BaseCam::StreamCapabilities::DEPTH) {
//            cv::Mat image = cv::Mat(height, width, CV_32FC1, bytes).clone();
//            return image;
//        }else if(type== BaseCam::StreamCapabilities::COLOR){
//            cv::Mat image = cv::Mat(height, width, CV_8UC4, bytes).clone();
//            return image;
//        }
//    }
//	static const boost::posix_time::ptime EPOCH;
//};
//
//class TCPConnectionServer : public boost::enable_shared_from_this<TCPConnectionServer>{
//    string message;
//    std::mutex serverMutex;
//protected:
//    tcp::socket socket;
//    std::string data;
//public:
//    typedef boost::shared_ptr<TCPConnectionServer> pointer;
//    static boost::shared_ptr<TCPConnectionServer> Create(boost::asio::io_service &io_service);
//    TCPConnectionServer(boost::asio::io_service &ioService);
//    tcp::socket &GetSocket();
//    void SendHandshakeRequest(std::vector<NetCamCVMatParams> netCamCVMatParams);
//    vector<BaseCam::StreamCapabilities>  ReceiveHandshakeAck();
//    void Send(cv::Mat& mat);
//    void SendDemoText();
//    void SendAsString(cv::Mat& mat);
//    void SendAsStream(cv::Mat &mat, BaseCam::StreamCapabilities streamType);
//    void SendFramePacket(cv::Mat mat, BaseCam::StreamCapabilities streamType);
//    void HandleWrite(const boost::system::error_code & error, size_t bytes_transferred);
//    void execute(cv::Mat mat, BaseCam::StreamCapabilities streamType);
//
//};
//
//class TCPConnectionClient : public boost::enable_shared_from_this<TCPConnectionClient> {
//protected:
//    tcp::socket socket;
//    std::string data;
//    NetCamCVMatParams netCamCVMatParams[MAX_NO_OF_SUPPORTED_STREAM_TYPES];
//    BaseCam* netcam;
//    std::mutex clientMutex;
//public:
//    bool readNextMessage=false;
//    typedef boost::shared_ptr<TCPConnectionClient> pointer;
//    static boost::shared_ptr<TCPConnectionClient> Create(boost::asio::io_service &io_service, BaseCam* netcam);
//    tcp::socket &GetSocket();
//    TCPConnectionClient(boost::asio::io_service &ioService, BaseCam* netcam);
//    virtual void execute();
//    void ReceiveHandshakeRequest();
//    void SendHandshakeAck(std::vector<BaseCam::StreamCapabilities> requestStreams);
//    void Receive();
//    void ReceiveDemoText();
//    void ReceiveAsString();
//    void ReceiveAsStream();
//    void ReceiveFramePacket();
//    void HandleRead(const boost::system::error_code & error, size_t bytes_transferred);
//};
//
//
//#endif //DYNCAM_TCPCONNECTION_H
