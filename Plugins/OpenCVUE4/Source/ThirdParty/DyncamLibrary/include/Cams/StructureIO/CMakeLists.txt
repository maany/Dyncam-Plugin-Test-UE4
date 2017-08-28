
MESSAGE("Adding Structure.io support")
# TODO chris
# OpenNI2
#LIST(APPEND CMAKE_MODULE_PATH /home/ascadian/Bremen/libfreenect2/cmake_modules/)
#find_package(OpenNI2 REQUIRED)

add_library(libStructureIO
    OpenNICam.cpp
    OpenNICam.h)

set_property(TARGET libStructureIO PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET libStructureIO PROPERTY CXX_STANDARD 14)

target_include_directories(libStructureIO PUBLIC ${OpenNI2_INCLUDE_DIRS})

find_package(Threads REQUIRED)
target_link_libraries(libStructureIO 
		Threads::Threads 
		OpenNI2)

target_include_directories(libStructureIO PRIVATE
        ${PROJECT_SOURCE_DIR}/deps/cpp.react/include)

#message(${OpenNI2_INCLUDE_DIRS})
#target_include_directories(libStructureIO SYSTEM ${OpenNI2_INCLUDE_DIRS})
#link_directories(${OPENNI2_LIBRARY_DIR})
