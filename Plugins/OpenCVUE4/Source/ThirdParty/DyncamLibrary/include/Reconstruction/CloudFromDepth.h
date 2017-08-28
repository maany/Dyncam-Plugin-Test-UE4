//
// Created by ascadian on 8/8/17.
//

#pragma once

#include "../ReactDomain.h"
#include <opencv2/opencv.hpp>
#include <src/Cams/Intrinsics.h>
#include <src/Cams/BaseCam.h>


class CloudFromDepth {

public:
    explicit CloudFromDepth(std::shared_ptr<BaseCam> cam);

protected:
    std::shared_ptr<BaseCam> m_cam;

    /* Given pixel coordinates and depth in an image with no distortion or inverse distortion
     * coefficients, compute the corresponding point in 3D space relative to the same camera */
    inline static void rs_deproject_pixel_to_point(const cam_intrinsics_t &intrin,
                                                   const int row, const int col,
                                                   const float depth_value,
                                                   float *res);

    static EventsT<cv::Mat >
    transformColor(EventsT<cv::Mat> &color);

    static EventsT<cv::Mat >
    transformDepth(EventsT<cv::Mat> &depth, VarSignalT<cam_intrinsics_t> &intrinsics);

public:
    EventsT<cv::Mat> PointPosEvents;
    EventsT<cv::Mat> PointColorsEvents;
};


