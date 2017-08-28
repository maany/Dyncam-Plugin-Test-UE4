
set(RECONSTRUCT_SOURCE
    CloudFromDepth.cpp
    CloudFromDepth.h)

add_library(libReconstruct
        ${RECONSTRUCT_SOURCE})

set_property(TARGET libReconstruct PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET libReconstruct PROPERTY CXX_STANDARD 14)

target_include_directories(libReconstruct PRIVATE
        ${PROJECT_SOURCE_DIR}/deps/cpp.react/include
		${PROJECT_SOURCE_DIR}
	)

target_include_directories(libReconstruct PUBLIC
        ${OpenCV_INCLUDE_DIRS}
	)
		
target_link_libraries(libReconstruct CppReact)