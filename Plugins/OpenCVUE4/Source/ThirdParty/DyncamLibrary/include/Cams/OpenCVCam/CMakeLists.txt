
add_library(libOpenCVCam
    OpenCVCam.cpp
    OpenCVCam.h)

set_property(TARGET libOpenCVCam PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET libOpenCVCam PROPERTY CXX_STANDARD 14)



target_include_directories(libOpenCVCam PRIVATE
        ${PROJECT_SOURCE_DIR}/deps/cpp.react/include
		${OpenCV_INCLUDE_DIRS})
