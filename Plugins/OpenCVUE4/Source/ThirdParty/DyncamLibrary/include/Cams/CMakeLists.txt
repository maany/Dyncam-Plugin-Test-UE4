
add_library(libcams
        BaseCam.cpp
        BaseCam.h
        Intrinsics.h)
set_property(TARGET libcams PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET libcams PROPERTY CXX_STANDARD 14)
target_include_directories(libcams PRIVATE
        ${PROJECT_SOURCE_DIR}/deps/cpp.react/include
		${OpenCV_INCLUDE_DIRS})

option(cam_structure "Build Structure.io support?" OFF)
if(cam_structure)
    add_subdirectory(StructureIO)
    target_link_libraries(libcams libStructureIO)
endif()

option(cam_freenect2 "Build freenect2 support?" ON)
if(cam_freenect2)
    add_subdirectory(Freenect2)
    target_link_libraries(libcams libFreenect2)
endif()

option(cam_openCV "Build opencv camera support?" ON)
if(cam_openCV)
    add_subdirectory(OpenCVCam)
    target_link_libraries(libcams libOpenCVCam)
endif()

option(cam_netcam "Build NetCam support?" ON)
if(cam_netcam)
    add_subdirectory(NetCam)
    target_link_libraries(libcams libNetCam)
endif()