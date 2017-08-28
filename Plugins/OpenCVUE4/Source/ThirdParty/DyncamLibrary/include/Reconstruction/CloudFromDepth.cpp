//
// Created by ascadian on 8/8/17.
//

#include "CloudFromDepth.h"


CloudFromDepth::CloudFromDepth(std::shared_ptr<BaseCam> cam) : m_cam(std::move(cam)) {

    PointPosEvents = CloudFromDepth::transformDepth(m_cam->UndistortedDepthEventsSource,
                                               m_cam->IntrinsicsIRSignal);

    const auto capabilities = m_cam->getStreamCapabilities();
    if (capabilities.find(BaseCam::StreamCapabilities::COLOR) != capabilities.end()) {
        PointColorsEvents = transformColor(m_cam->RegisteredColorEventsSource);
    }
}


inline void
CloudFromDepth::rs_deproject_pixel_to_point(const cam_intrinsics_t &intrin, const int row,
                                            const int col, const float depth_value, float *res) {
    auto depth = depth_value / 1000.0f; // scale to 1m
    const float bad_point = std::numeric_limits<float>::quiet_NaN();
    if (isnan(depth) || depth <= 0.001) {
        //depth_value value is not valid
        res[0] = bad_point;
        res[1] = bad_point;
        res[2] = bad_point;
    } else {
        // 3d camera projection
        float x = (col + 0.5f - intrin.cx) / intrin.fx;
        float y = (row + 0.5f - intrin.cy) / intrin.fy;

//        // undistort
//        float r2  = x*x + y*y;
//        float f = 1 + intrin.k1*r2 + intrin.k2*r2*r2 + intrin.k3*r2*r2*r2;
//        float ux = x*f + 2*intrin.p1*x*y + intrin.p2*(r2 + 2*x*x);
//        float uy = y*f + 2*intrin.p2*x*y + intrin.p1*(r2 + 2*y*y);
//        x = ux;
//        y = uy;

        res[0] = depth * x;
        res[1] = depth * y;
        res[2] = depth;
    }
}

EventsT<cv::Mat>
CloudFromDepth::transformColor(EventsT<cv::Mat> &color) {
    return Transform(color, [](const cv::Mat &image) {
        // allocate result matrix. Each row is one vertex color
        const auto cols = image.cols;
        const auto rows = image.rows;
        const auto points = rows * cols;
        cv::Mat res(points, 4, CV_32F);

        // process each pixel in parallel
        //typedef cv::TypeDepth<CV_8UC4>::value_type data_type;
        //using data_type = data_type;
        using data_type = cv::Vec4b;
        image.forEach<data_type>(
                [cols, &res](data_type pixel, const int *position) -> void {
                    // rows cols are change. opencv (.at(y,x)) or other reason?
                    const int y = position[0];
                    const int x = position[1];
                    // linearize point cloud
                    const int i = (x + y * cols);
                    res.at<float>(i, 0) = pixel[2] / 255.0f; // switch r and b
                    res.at<float>(i, 1) = pixel[1] / 255.0f;
                    res.at<float>(i, 2) = pixel[0] / 255.0f;
                    res.at<float>(i, 3) = pixel[3] / 255.0f;
                }
        );

        return res;
    });
}

EventsT<cv::Mat>
CloudFromDepth::transformDepth(EventsT<cv::Mat> &depth, VarSignalT<cam_intrinsics_t> &intrinsics) {
    return Transform(depth, With(intrinsics),
                     [](const cv::Mat &image, const cam_intrinsics_t &intr) {

                         const auto cols = image.cols;
                         const auto rows = image.rows;
                         const auto points = rows * cols;
                         cv::Mat res(points, 3, CV_32F);

                         // process each pixel in parallel
                         //typedef cv::TypeDepth<CV_32FC1>::value_type data_type;
                         using data_type = float;
                         image.forEach<data_type>(
                                 [cols, &intr, &res](data_type pixel, const int *position) -> void {
                                     // rows cols are change. opencv (.at(y,x)) or other reason?
                                     const int y = position[0];
                                     const int x = position[1];
                                     // linearize point cloud
                                     const int i = (x + y * cols);
                                     auto addr = &res.at<float>(i, 0);
                                     // last parameter is the adress where the result triple will be written to
                                     CloudFromDepth::rs_deproject_pixel_to_point(intr, x, y, pixel,
                                                                                 addr);
                                 }
                         );

                         return res;
	});
}
