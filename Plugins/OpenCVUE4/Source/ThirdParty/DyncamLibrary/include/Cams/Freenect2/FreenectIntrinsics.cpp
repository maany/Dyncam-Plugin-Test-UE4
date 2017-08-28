//
// Created by ascadian on 8/17/17.
//

#include "FreenectIntrinsics.h"


cam_intrinsics_t intrinsicsFreenectIr2Base(libfreenect2::Freenect2Device::IrCameraParams freenectIntr) {
    cam_intrinsics_t res;
    res.fx = freenectIntr.fx;
    res.fy = freenectIntr.fy;
    res.cx = freenectIntr.cx;
    res.cy = freenectIntr.cy;
    res.k1 = freenectIntr.k1;
    res.k2 = freenectIntr.k2;
    res.k3 = freenectIntr.k3;
    res.p1 = freenectIntr.p1;
    res.p2 = freenectIntr.p2;

    return res;
}

cam_intrinsics_t intrinsicsFreenectColor2Base(libfreenect2::Freenect2Device::ColorCameraParams freenectIntr) {
    cam_intrinsics_t res;
    res.fx = freenectIntr.fx;
    res.fy = freenectIntr.fy;
    res.cx = freenectIntr.cx;
    res.cy = freenectIntr.cy;
    res.k1 = 0;
    res.k2 = 0;
    res.k3 = 0;
    res.p1 = 0;
    res.p2 = 0;

    return res;
}


libfreenect2::Freenect2Device::IrCameraParams intrinsicsBase2FreenectIR(cam_intrinsics_t baseIntr) {
    libfreenect2::Freenect2Device::IrCameraParams res;
    res.fx = baseIntr.fx;
    res.fy = baseIntr.fy;
    res.cx = baseIntr.cx;
    res.cy = baseIntr.cy;
    res.k1 = baseIntr.k1;
    res.k2 = baseIntr.k2;
    res.k3 = baseIntr.k3;
    res.p1 = baseIntr.p1;
    res.p2 = baseIntr.p2;

    return res;
}

libfreenect2::Freenect2Device::ColorCameraParams intrinsicsBase2FreenectColor(cam_intrinsics_t baseIntr) {
    libfreenect2::Freenect2Device::ColorCameraParams res;
    res.fx = baseIntr.fx;
    res.fy = baseIntr.fy;
    res.cx = baseIntr.cx;
    res.cy = baseIntr.cy;

    return res;
}