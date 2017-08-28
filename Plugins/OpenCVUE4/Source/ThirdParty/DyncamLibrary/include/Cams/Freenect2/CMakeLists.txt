
# For Kinect v2 support.
find_package(freenect2 REQUIRED)

add_library(libFreenect2
    Freenect2Cam.cpp
    Freenect2Cam.h
    Freenect2CamManager.cpp
    Freenect2CamManager.h
    FreenectIntrinsics.h
    FreenectIntrinsics.cpp)

set_property(TARGET libFreenect2 PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET libFreenect2 PROPERTY CXX_STANDARD 14)

target_include_directories(libFreenect2 PUBLIC 
	${freenect2_INCLUDE_DIRS}
	${OpenCV_INCLUDE_DIRS})

target_link_libraries(libFreenect2
    ${freenect2_LIBRARY})

# FIXME: I guess we do not specifically depend on pthreads but rather need any threading lib?
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(libFreenect2 Threads::Threads)

target_include_directories(libFreenect2 PRIVATE
    ${PROJECT_SOURCE_DIR}/deps/cpp.react/include)
