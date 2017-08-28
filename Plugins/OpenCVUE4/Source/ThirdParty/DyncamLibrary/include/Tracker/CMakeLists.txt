
# Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

set(TRACKER_SOURCE
	BaseTracker.cpp
	BaseTracker.h
	PatternTracker.cpp
	PatternTracker.h)

add_library(tracking
        ${TRACKER_SOURCE})

set_property(TARGET tracking PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET tracking PROPERTY CXX_STANDARD 14)

target_include_directories(tracking PRIVATE
	${PROJECT_SOURCE_DIR}/deps/cpp.react/include)

target_include_directories(tracking PUBLIC
	${OpenCV_INCLUDE_DIRS})