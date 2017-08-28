//
// Created by ascadian on 8/9/17.
//

#pragma once

// http://docs.opencv.org/3.1.0/d9/d0c/group__calib3d.html#ga687a1ab946686f0d85ae0363b5af1d7b
typedef struct cam_intrinsics
{
    float fx; ///< Focal length x (pixel)
    float fy; ///< Focal length y (pixel)
    float cx; ///< Principal point x (pixel)
    float cy; ///< Principal point y (pixel)
    float k1; ///< Radial distortion coefficient, 1st-order
    float k2; ///< Radial distortion coefficient, 2nd-order
    float k3; ///< Radial distortion coefficient, 3rd-order
    float p1; ///< Tangential distortion coefficient
    float p2; ///< Tangential distortion coefficient0

    bool operator==(const cam_intrinsics &rhs) const {
        return    fx == rhs.fx
               && fy == rhs.fy
               && cx == rhs.cx
               && cy == rhs.cy
               && k1 == rhs.k1
               && k2 == rhs.k2
               && k3 == rhs.k3
               && p1 == rhs.p1
               && p2 == rhs.p2;
    }


} cam_intrinsics_t;


