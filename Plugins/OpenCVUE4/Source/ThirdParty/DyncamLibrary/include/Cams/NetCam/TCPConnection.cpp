////
//// Created by devmaany on 08.08.17.
////
//
//#include "TCPConnection.h"
////#include "../BaseCam.h"
//
//const boost::posix_time::ptime Util::EPOCH = boost::posix_time::ptime((boost::gregorian::date(2017, 1, 1)));
//
//NetCamCVMatParams::NetCamCVMatParams(BaseCam::StreamCapabilities streamType, int rows, int cols, int channels,
//                                     int cvMatType,
//                                     int frameLength) : streamType(static_cast<int>(streamType)), rows(rows),
//                                                        cols(cols), channels(channels), cvMatType(cvMatType),
//                                                        frameLength(frameLength) {
//
//}
//
//std::vector<NetCamCVMatParams> NetCamCVMatParams::Freenect2CVMatParams() {
//    std::vector<NetCamCVMatParams> freenect2CamCVMatParams;
//    NetCamCVMatParams freenectDepthCVMatParams(BaseCam::StreamCapabilities::DEPTH, 424, 512, 1, CV_32FC1, 868352);
//    NetCamCVMatParams freenectColorCVMatParams(BaseCam::StreamCapabilities::COLOR, 1080, 1920, 4, CV_8UC4, 8294400);
//    NetCamCVMatParams freenectIRCVMatParams(BaseCam::StreamCapabilities::IR, 424, 512, 1, CV_32FC1, 868352); //TODO check and update the values
//    freenect2CamCVMatParams.push_back(freenectDepthCVMatParams);
//    freenect2CamCVMatParams.push_back(freenectColorCVMatParams);
//    freenect2CamCVMatParams.push_back(freenectIRCVMatParams);
//    return freenect2CamCVMatParams;
//}
//
//const char* NetCamCVMatParams::ToString() {
//
//    string netCamCVMatParams =
//            std::to_string(streamType) + "," + std::to_string(rows) + "," + std::to_string(cols) + "," +
//            std::to_string(channels) + "," + std::to_string(cvMatType) + "," + std::to_string(frameLength);
//    const char* data = (const char *)malloc(sizeof(const char*)*netCamCVMatParams.length());
//    memcpy((void *) data, netCamCVMatParams.data(), netCamCVMatParams.length());
//    return data;
//}
//
//void NetCamCVMatParams::LoadFromString(NetCamCVMatParams& params,char *string) {
//    std::vector<std::string> values = Util::split(string, ',');
//    if (values.size() != 6)
//        throw std::length_error(
//                "[NetCamCVMatParams] : Cannot Load String as the data does not contain 6 csv values for representing streamType, rows, cols, channels, cvMatType, frameLength");
//    params.streamType = atoi(values.at(0).c_str());
//    params.rows = atoi(values.at(1).c_str());
//    params.cols = atoi(values.at(2).c_str());
//    params.channels = atoi(values.at(3).c_str());
//    params.cvMatType = atoi(values.at(4).c_str());
//    params.frameLength = atoi(values.at(5).c_str());
//}
//
//NetCamCVMatParams::NetCamCVMatParams() {
//    streamType = -1;
//    rows = -1;
//    cols = -1;
//    channels = -1;
//    cvMatType = -1;
//    frameLength = -1;
//}
//// ------------------------------------------
////            TCPConnectionServer
//// ------------------------------------------
//
//TCPConnectionServer::TCPConnectionServer(boost::asio::io_service &ioService) : socket(ioService) {
//
//}
//
//boost::shared_ptr<TCPConnectionServer> TCPConnectionServer::Create(boost::asio::io_service &io_service) {
//    return pointer(new TCPConnectionServer(io_service));
//}
//
//tcp::socket &TCPConnectionServer::GetSocket() {
//    return socket;
//}
//
//void TCPConnectionServer::execute(cv::Mat mat, BaseCam::StreamCapabilities streamType) {
////    Send(mat);
////    SendDemoText();
////    SendAsString(mat);
////    SendFramePacket(mat,BaseCam::StreamCapabilities::DEPTH);
//    SendAsStream(mat, streamType);
//}
//
//void TCPConnectionServer::SendHandshakeRequest(std::vector<NetCamCVMatParams> netCamCVMatParams) {
//    boost::asio::streambuf handshakeBuf;
//    std::ostream handshakeRequestStream(&handshakeBuf);
//    handshakeRequestStream << netCamCVMatParams.size() << "\n";
//    for(uint i=0; i<netCamCVMatParams.size(); i++){
//        const char* data = netCamCVMatParams.at(i).ToString();
//        cout<<"[Handshake Request] : Writing : "<<data<<endl;
//        handshakeRequestStream<<data<<"\n";
//        free((void*)data);
//    }
//    handshakeRequestStream<<"\r\n";
//    boost::asio::write(socket,handshakeBuf);
//}
//
//vector<BaseCam::StreamCapabilities>  TCPConnectionServer::ReceiveHandshakeAck(){
//    vector<BaseCam::StreamCapabilities> requestedStreams;
//    boost::asio::streambuf handshakeAckBuf;
//    boost::asio::read_until(socket, handshakeAckBuf, "\r\n");
//    std::istream handshakeAckStream(&handshakeAckBuf);
//    unsigned int noOfRequestedStreams;
//    handshakeAckStream >> noOfRequestedStreams;
//    handshakeAckStream.get();
//    cout<<"[TCPConnectionServer] : No of Requested Streams : "<<noOfRequestedStreams<<endl;
//    for(uint i=0;i<noOfRequestedStreams;i++){
//        string requestedStreamAsString;
//        std::getline(handshakeAckStream,requestedStreamAsString);
//        int requstedStream = atoi(requestedStreamAsString.data());
//        if(requstedStream == static_cast<int>(BaseCam::StreamCapabilities::DEPTH)){
//            cout<<"[TCPConnectionServer] : Requested Depth"<<endl;
//            requestedStreams.push_back(BaseCam::StreamCapabilities::DEPTH);
//        }else if(requstedStream == static_cast<int>(BaseCam::StreamCapabilities::COLOR)){
//            requestedStreams.push_back(BaseCam::StreamCapabilities::COLOR);
//            cout<<"[TCPConnectionServer] : Requested Color"<<endl;
//        }else if(requstedStream == static_cast<int>(BaseCam::StreamCapabilities::IR)){
//            requestedStreams.push_back(BaseCam::StreamCapabilities::IR);
//            cout<<"[TCPConnectionServer] : Requested IR"<<endl;
//        }
//    }
//    return requestedStreams;
//}
//void TCPConnectionServer::SendDemoText() {
//    message = "Demo Text";
//    cout << "[TCPConnectionServer] : " << "sending -> " << message << endl;
//    boost::asio::async_write(socket, boost::asio::buffer(message),
//                             boost::bind(&TCPConnectionServer::HandleWrite, shared_from_this(),
//                                         boost::asio::placeholders::error,
//                                         boost::asio::placeholders::bytes_transferred));
//}
//
//void TCPConnectionServer::SendAsString(cv::Mat &mat) {
//    // UnComment for Depth Packet
//    FramePacket packet;
//    packet.dataType = "depth";
//    packet.data = string((char *) mat.data, Util::GetCVMatSize(mat.reshape(0, 1)));
//    cout << "[TCPConnectionServer] : " << "sending frame packet -> " << packet.dataType << endl;
//    string toSend = packet.saveAsString();
//    boost::asio::write(this->GetSocket(), boost::asio::buffer(toSend));
//
//    // UnComment for Color packet
//    /*FramePacket packet;
//    packet.dataType = "color";
//    packet.data = string((char*)mat.data, GetCVMatSize(mat.reshape(0, 1)));
//    cout << "[TCPConnectionServer] : " << "sending frame packet -> " <<packet.dataType<< endl;
//    string toSend = packet.saveAsString();
//    boost::asio::write(this->GetSocket(),boost::asio::buffer(toSend));*/
//}
//
//void TCPConnectionServer::SendFramePacket(cv::Mat mat, BaseCam::StreamCapabilities streamType) {
//    //Depth using ostream
//    FramePacket packet;
//    if (streamType == BaseCam::StreamCapabilities::DEPTH) {
//        packet.dataType = "depth";
//    } else if (streamType == BaseCam::StreamCapabilities::COLOR) {
//        packet.dataType = "color";
//    }
//    packet.data = string((char *) mat.data, Util::GetCVMatSize(mat.reshape(0, 1)));
//    cout << "[TCPConnectionServer] : " << "sending frame packet -> " << packet.dataType << endl;
//    boost::asio::streambuf request;
//    std::ostream request_stream(&request);
//    packet.SaveFramePacket(request_stream);
//    boost::asio::write(this->GetSocket(), request);
//}
//
//void TCPConnectionServer::SendAsStream(cv::Mat &mat, BaseCam::StreamCapabilities streamType) {
//    boost::asio::streambuf request;
//    std::ostream request_stream(&request);
//
//    boost::asio::streambuf matbuf;
//    std::ostream data_stream(&matbuf);
//    // Send streamType
//	int type = static_cast<int>(streamType);
//
//	//string typeString = std::to_string(static_cast<int>(streamType));
//	//const char* type = typeString.c_str();
//    /*if (streamType == BaseCam::StreamCapabilities::DEPTH)
//        type = std::to_string(0).c_str();
//    else if (streamType == BaseCam::StreamCapabilities::COLOR)
//        type = std::to_string(1).c_str();*/
//
//	//request_stream.write(type, strlen(type));
//	cout << "[Socket Server] : Type = " << type<<"\n";
//	request_stream << type << "\n";
//	//Send timestamp
//	//std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
//	/*	std::chrono::system_clock::now().time_since_epoch()
//	);
//	long time = ms.count();*/
//	long time = (boost::posix_time::second_clock::universal_time() - Util::EPOCH).total_milliseconds();
//
//	//request_stream<<ms.count();
//	//string timeString = std::to_string(ms.count());
//	//const char *time = timeString.c_str();
//    //request_stream.write(time, std::strlen(time));
//	cout << "[Socket Server] : TimeStamp = " << time <<"\n";
//	request_stream << time << "\n";
//	request_stream << "\r\n";
//    //Send matrix in next go.
//    if (mat.isContinuous())
//        data_stream.write((char *) mat.data, Util::GetCVMatSize(mat));
//    else {
//        size_t rowsz = mat.elemSize() * mat.cols;
//        for (int r = 0; r < mat.rows; ++r)
//            data_stream.write((char *) mat.ptr<char>(r), rowsz);
//    }
////    request_stream.write("\r\n",4);
//    data_stream.flush();
//	request_stream.flush();
//	{
//		std::lock_guard<std::mutex> lock(serverMutex);
//		cout << "[Socket Server] : Sends Depth " << endl;
//		cout << "[Socket Server] : Size of request " << request.size() << endl;
//		boost::asio::write(socket, request);
//            cout << "[Socket Server] : Size of mat " << matbuf.size() << endl;
//            boost::asio::write(socket, matbuf);
//    }
//
//}
//
//void TCPConnectionServer::Send(cv::Mat &mat) {
//
//
//    message = string((char *) mat.data, Util::GetCVMatSize(mat.reshape(0, 1)));
//    cout << "[NetCamServer] : Transmitting message of size : " << message.size() << endl;
//    /*boost::asio::async_write(m_Socket, boost::asio::buffer(message),
//                              boost::bind(&TCPConnectionServer::HandleWrite, shared_from_this(),
//                                          boost::asio::placeholders::error,
//                                          boost::asio::placeholders::bytes_transferred));*/
//
//    this->GetSocket().write_some(boost::asio::buffer(message));
//}
//
//void TCPConnectionServer::HandleWrite(const boost::system::error_code &error,
//                                      size_t bytes_transferred) {
//    if (!error) {
//        cout << "[NetCamServer] : " << "Frame transmitted successfully" << endl;
//        cout << "[NetCamServer] : " << "Bytes transferred : " << bytes_transferred << endl;
//    } else {
//        cout << "[NetCamServer] : " << "Error in Transmission : " << error.message() << endl;
//        cout << "[NetCamServer] : " << "Bytes Transmitted : " << bytes_transferred << endl;
//    }
//}
//
//// ------------------------------------------
////            TCPConnectionClient
//// ------------------------------------------
//
//
//TCPConnectionClient::TCPConnectionClient(boost::asio::io_service &ioService, BaseCam* netcam) : socket(ioService), netcam(netcam) {
//
//}
//
//boost::shared_ptr<TCPConnectionClient> TCPConnectionClient::Create(boost::asio::io_service &io_service, BaseCam* netcam) {
//    TCPConnectionClient* tcpConnectionClient= new TCPConnectionClient(io_service,netcam);
//    return pointer(tcpConnectionClient);
//}
//
//tcp::socket &TCPConnectionClient::GetSocket() {
//    return socket;
//}
//
//void TCPConnectionClient::execute() {
//    readNextMessage = false;
//    cout << "[TCPConnectionClient] : " << "Ready to Receive " << endl;
////	ReceiveAsString();
////    ReceiveFramePacket();
//    ReceiveAsStream();
//}
//
//void TCPConnectionClient::ReceiveHandshakeRequest(){
//    boost::asio::streambuf handshakeResponse;
//    boost::asio::read_until(socket, handshakeResponse, "\r\n");
//    std::istream handshakeResponseStream(&handshakeResponse);
//    unsigned int number_of_streams;
//    handshakeResponseStream >> number_of_streams;
//    handshakeResponseStream.get();
//    cout<<"number of streams :: "<<number_of_streams<<endl;
//    for (uint i=0;i<number_of_streams;i++){
//        string netCamCVMatData;
//        std::getline(handshakeResponseStream,netCamCVMatData);
//        NetCamCVMatParams params;
//        NetCamCVMatParams::LoadFromString(params,const_cast<char *>(netCamCVMatData.c_str()));
//        netCamCVMatParams[params.streamType] = params;
//        if(params.streamType == static_cast<int>(BaseCam::StreamCapabilities::DEPTH)){
//            cout<<"[TCPConnectionClient] Depth Stream Available : "<<netCamCVMatData<<endl;
//        }else if(params.streamType == static_cast<int>(BaseCam::StreamCapabilities::COLOR)){
//            cout<<"[TCPConnectionClient] Color Stream Available : "<<netCamCVMatData<<endl;
//        }else if(params.streamType == static_cast<int>(BaseCam::StreamCapabilities::IR)){
//            cout<<"[TCPConnectionClient] IR Stream Available : "<<netCamCVMatData<<endl;
//        }
//    }
//}
//void TCPConnectionClient::SendHandshakeAck(std::vector<BaseCam::StreamCapabilities> requestStreams){
//    boost::asio::streambuf handshakeAckBuf;
//    std::ostream handshakeAckStream(&handshakeAckBuf);
//    handshakeAckStream<<requestStreams.size()<<"\n";
//    for(uint i =0; i<requestStreams.size();i++){
//        int data = static_cast<int>(requestStreams.at(i));
//        handshakeAckStream<<data<<"\n";
//    }
//    handshakeAckStream<<"\r\n";
//    boost::asio::write(socket, handshakeAckBuf);
//}
//void TCPConnectionClient::Receive() {
///*boost::asio::async_read(m_Socket, boost::asio::buffer(buf),
//                            boost::bind(&TCPConnectionClient::HandleRead, shared_from_this(),
//                                        boost::asio::placeholders::error,
//                                        boost::asio::placeholders::bytes_transferred));*/
//
//    boost::system::error_code ec;
////    boost::array<char, (868352)> buf;
//    boost::array<char, (8294406)> buf;
////    boost::array<char, (9)> buf;
//    boost::asio::read(socket, boost::asio::buffer(buf), ec);
////    this->GetSocket().read_some(boost::asio::buffer(buf), ec);
//    if (!ec) {
//        cout << "[NetCam] : buffer data : " << buf.data() << endl;
//        std::vector<uchar> vectordata(buf.begin(),
//                                      buf.end()); /* change the recieved mat frame(1*230400) to vector */
//        cv::Mat data_mat(vectordata, true);
//        cout << "[NetCam] : " << "Rows, Cols, Channels " << data_mat.rows << " , " << data_mat.cols
//             << " , "
//             << data_mat.channels() << endl;
//        cv::Mat img = cv::Mat(424, 512, CV_8UC4);//, data_mat.data);
//        img = data_mat.reshape(1, 424);
//        cv::imshow("Client Depth", img);
//        cv::waitKey(1);
//    } else {
//        cout << "[NetCam] : Error in receiving data : " << ec.message();
//    }
//    readNextMessage = true;
//}
//
//void TCPConnectionClient::ReceiveDemoText() {
//    boost::system::error_code ec;
//    boost::array<char, (9)> buf;
//    boost::asio::read(socket, boost::asio::buffer(buf), ec);
//    if (!ec) {
//        cout << "[NetCam] : buffer data : " << buf.data() << endl;
//    } else {
//        cout << "[NetCam] : Error in receiving data : " << ec.message();
//    }
//    readNextMessage = true;
//}
//
//void TCPConnectionClient::ReceiveAsString() {
//    boost::system::error_code ec;
//    //TODO adjust buffer based on datatype
//
////    boost::array<char, (8294406)> buf; // color
//    boost::array<char, 868358> buf;
//    FramePacket packet;
//    boost::asio::read(socket, boost::asio::buffer(buf), ec);
//    if (!ec) {
//        string message = string(buf.data());
//        std::copy(buf.begin(), buf.end(), std::back_inserter(message));
//        packet.LoadAsString(message);
//        cout << "[NetCam] : buffer dataType : " << packet.dataType << endl;
//        if (packet.dataType == "depth") {
//            std::cout << "Size of data = " << sizeof(packet.data) << endl;
//            cout << packet.data << endl;
//            std::vector<uchar> vectordata(buf.begin() + 6,
//                                          buf.end()); //* change the recieved mat frame(1*230400) to vector *//*
//            cv::Mat data_mat(vectordata, true);
//            cout << "[NetCam] : " << "Rows, Cols, Channels " << data_mat.rows << " , " << data_mat.cols
//                 << " , "
//                 << data_mat.channels() << endl;
//            cv::Mat img = cv::Mat(424, 512, CV_32FC1);
//            img = data_mat.reshape(1, 424);
//            cv::imshow("Client Depth", img);
//            cv::waitKey(1);
//        } else if (packet.dataType == "color") {
//            std::cout << "Size of data = " << sizeof(packet.data) << endl;
//            cout << packet.data << endl;
//            std::vector<uchar> vectordata(buf.begin() + 6,
//                                          buf.end()); //* change the recieved mat frame(1*230400) to vector *//*
//            cv::Mat data_mat(vectordata, true);
//            cout << "[NetCam] : " << "Rows, Cols, Channels " << data_mat.rows << " , " << data_mat.cols
//                 << " , "
//                 << data_mat.channels() << endl;
//            cv::Mat img = cv::Mat(1080, 1920, CV_8UC4);
//            img = data_mat.reshape(4, 1080);
//            cv::imshow("Client Depth", img);
//            cv::waitKey(1);
//        }
//    } else {
//        cout << "[NetCam] : Error in receiving data : " << ec.message();
//    }
//    readNextMessage = true;
//}
//
//void TCPConnectionClient::ReceiveFramePacket() {
//    boost::asio::streambuf response;
//    boost::asio::read_until(socket, response, "\r\n");
//    std::istream response_stream(&response);
//    FramePacket *packet = FramePacket::AssemblePacket(response_stream);
//    if (packet == nullptr) {
//        readNextMessage = true;
//        return; // keep reading
//    } else {
//        // ready for processing;
//        cout << "[NetCam] : buffer data type : " << packet->dataType << endl;
//        cout << "[NetCam] : buffer data length: " << packet->data.length() << endl;
//        if (packet->dataType == "depth") {
//            std::vector<uchar> vectordata(packet->data.begin(),
//                                          packet->data.end() - 3);
//            cv::Mat data_mat(vectordata, true);
//            cout << "[NetCam] : " << "Rows, Cols, Channels " << data_mat.rows << " , " << data_mat.cols
//                 << " , "
//                 << data_mat.channels() << endl;
//            cv::Mat img = cv::Mat(424, 512, CV_32FC1);
//            img = data_mat.reshape(1, 424);
//            cv::imshow("Client Depth", img);
//            cv::waitKey(3);
//        } else if (packet->dataType == "color") {
//            // Not stable
//            std::vector<uchar> vectordata(packet->data.begin(), packet->data.end() - 2);
//            cv::Mat data_mat(vectordata, true);
//            cout << "[NetCam] : " << "Rows, Cols, Channels " << data_mat.rows << " , " << data_mat.cols
//                 << " , "
//                 << data_mat.channels() << endl;
//            cv::Mat img = cv::Mat(1080, 1920, CV_8UC4);
//            img = data_mat.reshape(4, 1080);
//            cv::imshow("Client Color", img);
//            cv::waitKey(1);
//        }
//    }
//    readNextMessage = true;
//}
//
//void TCPConnectionClient::ReceiveAsStream() {
//    boost::asio::streambuf response;
//	{
//		std::lock_guard<std::mutex> lock(clientMutex);
////		boost::asio::read_until(m_Socket, response, ":");
//		boost::asio::read(socket, response, boost::asio::transfer_exactly(16));
//	}
//    // End Time
////    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
////            std::chrono::system_clock::now().time_since_epoch()
////    );
////    long endTime = ms.count();
//    long endTime = (boost::posix_time::second_clock::local_time() - Util::EPOCH).total_microseconds();
//    std::istream response_stream(&response);
////	char* readString;
////    response_stream.read(readString,18);
////    cout<<"Header Text: "<<readString<<endl;
//
//    unsigned int typeVal=100;
//	response_stream >> typeVal;
//	response_stream.get();
//	long startTime = 0;
//	response_stream >> startTime;
//	response_stream.get();
//	//char *type = new char[1];
//	//response_stream.read(type, 1);
//    cout << "[TCPConnectionClient] : Stream Type: " << typeVal << " : " << typeVal << endl;
//    //int typeVal = atoi(type);
//
//    //char *streambuf = new char[13];
//    //response_stream.read(streambuf, 13);
//    //long startingTime = atol(streambuf);
//    cout << "[TCPConnectionClient] : Transmission Delay : " << endTime - startTime << endl;
//	if (typeVal == 100)
//		return;
//    boost::asio::streambuf matrix;
//    std::istream matrix_stream(&matrix);
//
//    NetCamCVMatParams cvMatParams = netCamCVMatParams[typeVal];
//    boost::asio::read(socket, matrix, boost::asio::transfer_exactly(cvMatParams.frameLength));
//    cv::Mat mat(cvMatParams.rows, cvMatParams.cols, cvMatParams.cvMatType);
//	{
//		std::lock_guard<std::mutex> lock(clientMutex);
//		matrix_stream.read((char *)mat.data, cvMatParams.frameLength);
//
//		if (typeVal == static_cast<int>(BaseCam::StreamCapabilities::DEPTH)) {
//			cout << "[Stream] : Received Depth Packet" << endl;
//			netcam->DepthEventsSource.Emit(mat);
//			//        cv::imshow("Client Depth", mat);
//
//		}
//		else if (typeVal == static_cast<int>(BaseCam::StreamCapabilities::COLOR)) {
//			cout << "[Stream] : Received Color Packet" << endl;
//			netcam->ColorEventsSource.Emit(mat);
//			//        cv::imshow("Client Color", mat);
//		}
//		else if (typeVal == static_cast<int>(BaseCam::StreamCapabilities::IR)) {
//			cout << "[Stream] : Received Color Packet" << endl;
//			netcam->IrEventsSource.Emit(mat);
//			//        cv::imshow("Client IR", mat);
//		}
//		readNextMessage = true;
//	}
////    cv::waitKey(1);
//}
//
///*
//void TCPConnectionClient::HandleRead(const boost::system::error_code &error, size_t bytes_transferred) {
//
//    std::vector<uchar> vectordata(buf.begin(),buf.end());
//
//// change the recieved mat frame(1*230400) to vector *//*
//    std::copy(buf.begin(), buf.begin()+bytes_transferred, std::back_inserter(message));
//    message = buf.data();
//    cout << "BC BC " << message << endl;
//
//    if (!error) {
//        cout << "[TCPConnectionClient] : " << "Received : " << bytes_transferred << endl;
//        message = buf.data();
//        cout << message << endl;
//        boost::array<char,868353> data_;
//        m_Socket.read_some(boost::asio::buffer(data_));
//        cout << "[TCPConnectionClient] : " << "The message from m_Socket was : " << data_.data() << endl;
//    } else {
//        cout << "[TCPConnectionClient] : " << error.message() << endl;
//    }
////    cv::Mat data_mat(vectordata,true);
////    img= data_mat.reshape(3,240);
//    this->readNextMessage = true;
//
//}
//*/
//
////*********************************/
//void FramePacket::LoadAsString(const string message) {
//    vector<string> splitData = Util::split(message, ':');
//    dataType = splitData.at(0);
//    data = splitData.at(1);
//}
//
////*********************************//
//// Frame Packet
////********************************//
//
//FramePacket FramePacket::assembledFramePacket;
//
//std::string FramePacket::saveAsString() {
//    return dataType + ":" + data;
//}
//
//FramePacket *FramePacket::AssemblePacket(std::istream &response_stream) {
//    string streamdata;
//    copy(istreambuf_iterator<char>(response_stream), istreambuf_iterator<char>(), back_inserter(streamdata));
//    string metaInfo = streamdata.substr(0, 5);
//    if (metaInfo == "depth") {
//        cout << "[FramePacket] depth" << endl;
//        FramePacket::assembledFramePacket = FramePacket();
//        string startTime = streamdata.substr(5, 13);
//        assembledFramePacket.startTime = stol(startTime, nullptr, 10);
//        assembledFramePacket.dataType = "depth";
//        assembledFramePacket.data = streamdata.substr(17);
//    } else if (metaInfo == "color") {
//        cout << "[FramePacket] color" << endl;
//        FramePacket::assembledFramePacket = FramePacket();
//        string startTime = streamdata.substr(5, 13);
//        assembledFramePacket.startTime = stol(startTime, nullptr, 10);
//        assembledFramePacket.dataType = "color";
//        assembledFramePacket.data = streamdata.substr(18);
//    } else {
//        assembledFramePacket.data += streamdata;
//        cout << "[FramePacket] contd : " << assembledFramePacket.data.length() << endl;
//    }
//    if (assembledFramePacket.dataType == "depth" && assembledFramePacket.data.length() > 868352) {
//        boost::hash<std::string> string_hash;
//        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
//                std::chrono::system_clock::now().time_since_epoch()
//        );
//        assembledFramePacket.endTime = ms.count();
//        cout << "[TCPConnectionClient] : Transmission Delay : "
//             << assembledFramePacket.endTime - assembledFramePacket.startTime << endl;
//        cout << "[NetCam] : Hash Value : " << string_hash(assembledFramePacket.data.substr(0, -3))
//             << endl;
//        return &FramePacket::assembledFramePacket;
//    } else if (assembledFramePacket.dataType == "color" && assembledFramePacket.data.length() >= 8294400) {
//        // Not stable
//        boost::hash<std::string> string_hash;
//        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
//                std::chrono::system_clock::now().time_since_epoch()
//        );
//        assembledFramePacket.endTime = ms.count();
//        cout << "[TCPConnectionClient] : Transmission Delay : "
//             << assembledFramePacket.endTime - assembledFramePacket.startTime << endl;
//        cout << "[NetCam] : Hash Value : " << string_hash(assembledFramePacket.data.substr(0, -3))
//             << endl;
//        return &FramePacket::assembledFramePacket;
//    }
//    return nullptr;
//}
//
//void FramePacket::SaveFramePacket(std::ostream &request_stream) {
//    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
//            std::chrono::system_clock::now().time_since_epoch()
//    );
//    request_stream << dataType + std::to_string(ms.count()) + data << "\r\n";
//    request_stream.flush();
//    cout << "[NetCamServer] : sent data length: " << data.length() << endl;
//    boost::hash<std::string> string_hash;
//    cout << "[NetCamServer] : sent data hash: " << string_hash(data) << endl;
//}
