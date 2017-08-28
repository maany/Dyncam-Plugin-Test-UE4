# For NetCamServer (TCP Sockets) support.
set(Boost_USE_MULTITHREADED ON)
if(WIN32)
	set(Boost_USE_STATIC_LIBS ON)
	find_package(Boost REQUIRED COMPONENTS system date_time regex)
elseif(UNIX)
	find_package(Boost REQUIRED COMPONENTS system)
endif()

add_library(libNetCam
        TCPConnection.cpp
        TCPConnection.h
        NetCamServer.cpp
        NetCamServer.h
        NetCam.h
        NetCam.cpp TCPServer.cpp TCPServer.h TCPClient.cpp TCPClient.h TCPCommons.cpp TCPCommons.h)

set_property(TARGET libNetCam PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET libNetCam PROPERTY CXX_STANDARD 14)
if(WIN32)
	target_compile_definitions(libNetCam PUBLIC -DWIN32_LEAN_AND_MEAN) #required for boost asio only by windows
endif()
target_include_directories(libNetCam PUBLIC 
	${Boost_INCLUDE_DIRS}
	${OpenCV_INCLUDE_DIRS})
target_link_libraries(libNetCam ${Boost_LIBRARIES})
target_include_directories(libNetCam PRIVATE
        ${PROJECT_SOURCE_DIR}/deps/cpp.react/include)