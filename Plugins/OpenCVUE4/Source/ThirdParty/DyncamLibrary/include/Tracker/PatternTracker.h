
#ifndef PATTERNTRACKER_H
#define PATTERNTRACKER_H

#include "BaseTracker.h"
#include "../Cams/BaseCam.h"

class PatternTracker : public BaseTracker
{
public:
    PatternTracker( std::shared_ptr<BaseCam> cam, cv::Size patternSize, double patternSpacing );

    virtual EventsT<cv::Matx44d> start();

private:
    double computeReprojectionErrors(const std::vector<std::vector<cv::Point3f> >& objectPoints,
                                     const std::vector<std::vector<cv::Point2f> >& imagePoints,
                                     const std::vector<cv::Mat>& rvecs, const std::vector<cv::Mat>& tvecs,
                                     const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs,
                                     std::vector<float>& perViewErrors );

private:
    std::shared_ptr<BaseCam> m_cam;
    cv::Size m_patternSize;
    double m_patternSpacing;

    // this will be set to true once there were enough patterns recognized or the intrinsic calibration is done
    VarSignalT<bool> m_patternCollectionDone = MakeVar<D>(false);
    VarSignalT<bool> m_intrinsicCalibrationDone = MakeVar<D>(false);

    ObserverT m_calibrator;

    std::vector<std::vector<cv::Point2f>> m_allCenters; //this will be filled with the detected centers
    std::vector<cv::Point3f> m_corners;
    cv::Mat m_camMat = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat m_newCamMat = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat m_distCoeffs;
    cv::Mat m_rvec;
    cv::Mat m_tvec;

    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastPatternFound;
};

#endif // PATTERNTRACKER_H
