//
// Created by ascadian on 8/16/17.
//

#pragma once
#include "Freenect2Cam.h"

cam_intrinsics_t intrinsicsFreenectIr2Base(libfreenect2::Freenect2Device::IrCameraParams freenectIntr);

cam_intrinsics_t intrinsicsFreenectColor2Base(libfreenect2::Freenect2Device::ColorCameraParams freenectIntr);

libfreenect2::Freenect2Device::IrCameraParams intrinsicsBase2FreenectIR(cam_intrinsics_t baseIntr);

libfreenect2::Freenect2Device::ColorCameraParams intrinsicsBase2FreenectColor(cam_intrinsics_t baseIntr);
