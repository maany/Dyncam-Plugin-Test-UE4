
#include "PatternTracker.h"

#include <memory>

PatternTracker::PatternTracker(std::shared_ptr<BaseCam> cam, cv::Size patternSize, double patternSpacing)
    : m_cam( cam )
    , m_patternSize( patternSize )
    , m_patternSpacing( patternSpacing )
    , m_lastPatternFound( std::chrono::high_resolution_clock::now() )
{
    // calculate the corner positions on the pattern
    for( int i = 0; i < m_patternSize.height; i++ )
        for( int j = 0; j < m_patternSize.width; j++ )
            m_corners.push_back( cv::Point3f((2*j + i % 2)*m_patternSpacing, i*m_patternSpacing, 0 ));
}

EventsT<cv::Matx44d> PatternTracker::start()
{
    // let's see if we can find a file with the camera matrices and the distortion coefficients for this camera
    std::ifstream camFile( "camIntrinsics_" + m_cam->name() + ".dat" );
    if( camFile.is_open() )
    {
        camFile >> m_camMat.at<double>(0) >> m_camMat.at<double>(1) >> m_camMat.at<double>(2);
        camFile >> m_camMat.at<double>(3) >> m_camMat.at<double>(4) >> m_camMat.at<double>(5);
        camFile >> m_camMat.at<double>(6) >> m_camMat.at<double>(7) >> m_camMat.at<double>(8);

        camFile >> m_newCamMat.at<double>(0) >> m_newCamMat.at<double>(1) >> m_newCamMat.at<double>(2);
        camFile >> m_newCamMat.at<double>(3) >> m_newCamMat.at<double>(4) >> m_newCamMat.at<double>(5);
        camFile >> m_newCamMat.at<double>(6) >> m_newCamMat.at<double>(7) >> m_newCamMat.at<double>(8);

        cv::Size nrOfCoeffs(0,0);
        camFile >> nrOfCoeffs.width;
        camFile >> nrOfCoeffs.height;

        m_distCoeffs.create( nrOfCoeffs, CV_64F );
        for( int i=0; i<nrOfCoeffs.height; ++i )
            for( int j=0; j<nrOfCoeffs.width; ++j )
                camFile >> m_distCoeffs.at<double>(i, j);

        m_patternCollectionDone.Set( true );
        m_intrinsicCalibrationDone.Set( true );

        camFile.close();
    }

    // collect patterns from the IR image and undistort it when the intrinsic calibration was done
    auto irUndistorted =
    Transform(m_cam->IrEventsSource, [&](cv::Mat irImage) {
        double min;
        double max;
        cv::minMaxIdx(irImage, &min, &max);

        cv::Mat pat;
        irImage.convertTo(pat, CV_8UC1, 255.0 / (max - min), -min);

        auto timeSinceLastPattern = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - m_lastPatternFound);
        if( !m_patternCollectionDone.Value() && timeSinceLastPattern.count() > 0.5 )
        {
            std::vector<cv::Point2f> centers;
            bool patternfound = cv::findCirclesGrid( pat, m_patternSize, centers, cv::CALIB_CB_ASYMMETRIC_GRID );
            cv::drawChessboardCorners(pat, m_patternSize, cv::Mat(centers), patternfound);

            if( patternfound )
            {
                m_lastPatternFound = std::chrono::high_resolution_clock::now();
                m_allCenters.push_back( centers );
                if( m_allCenters.size() >= 20 )
                    m_patternCollectionDone.Set( true );

                std::cout << "Found pattern #" << m_allCenters.size() << std::endl;
            }
        }

        cv::Mat ret;
        if( m_intrinsicCalibrationDone.Value() )
            cv::undistort( pat, ret, m_camMat, m_distCoeffs, m_newCamMat );
        else
            ret = pat;

        return ret;
    });

    // Once we have enough patterns do the callibration
    m_calibrator =
    Observe( m_patternCollectionDone, [this] (bool done) {
        if( done )
        {
            // * calibrate the camera
            // create the object points vector
            std::vector<cv::Point3f> corners;
            for( int i = 0; i < m_patternSize.height; i++ )
                for( int j = 0; j < m_patternSize.width; j++ )
                    corners.push_back( cv::Point3f((2*j + i % 2)*20, i*20, 0 ));

            std::vector<std::vector<cv::Point3f>> objPoints( m_allCenters.size(), corners );

            // calibrate
            std::vector<cv::Mat> rvecs, tvecs;
            calibrateCamera( objPoints, m_allCenters, m_cam->m_resIR.Value(), m_camMat, m_distCoeffs, rvecs, tvecs );

            // compute the reprojection error
            std::vector<float> reprojErrs;
            double totalAvgErr = computeReprojectionErrors(objPoints, m_allCenters, rvecs, tvecs,
                                                           m_camMat, m_distCoeffs, reprojErrs);

            std::cout << "Total average reprojection error: " << totalAvgErr << std::endl;

            // compute the optimal, zoomed and cropped camera mat
            m_newCamMat = cv::getOptimalNewCameraMatrix( m_camMat, m_distCoeffs, m_cam->m_resIR.Value(), 0 );

            // write the results to a file
            std::ofstream camFile( "camIntrinsics_" + m_cam->name() + ".dat" );
            if( camFile.is_open() )
            {
                camFile << m_camMat.at<double>(0) << " " << m_camMat.at<double>(1) << " " << m_camMat.at<double>(2) << "\n";
                camFile << m_camMat.at<double>(3) << " " << m_camMat.at<double>(4) << " " << m_camMat.at<double>(5) << "\n";
                camFile << m_camMat.at<double>(6) << " " << m_camMat.at<double>(7) << " " << m_camMat.at<double>(8) << "\n";

                camFile << m_newCamMat.at<double>(0) << " " << m_newCamMat.at<double>(1) << " " << m_newCamMat.at<double>(2) << "\n";
                camFile << m_newCamMat.at<double>(3) << " " << m_newCamMat.at<double>(4) << " " << m_newCamMat.at<double>(5) << "\n";
                camFile << m_newCamMat.at<double>(6) << " " << m_newCamMat.at<double>(7) << " " << m_newCamMat.at<double>(8) << "\n";

                camFile << m_distCoeffs.size().width << " ";
                camFile << m_distCoeffs.size().height << "\n";

                for( int i=0; i<m_distCoeffs.size().height; ++i )
                    for( int j=0; j<m_distCoeffs.size().width; ++j )
                        camFile << m_distCoeffs.at<double>(i, j) << " ";

                camFile.close();
            }

            m_intrinsicCalibrationDone.Set( true );
        }
    });

    // retrieve the camera extrinsic transformation matrix whenever a pattern was found
    cv::namedWindow("Infrared and Pattern", CV_WINDOW_NORMAL);
    auto camToPattern =
    Transform( irUndistorted, [this](cv::Mat undistortedIR) {

        cv::Matx44d transMat = cv::Matx44d::eye();

        if( m_intrinsicCalibrationDone.Value() )
        {
            cv::Mat viewColor;
            cvtColor(undistortedIR, viewColor, cv::COLOR_GRAY2BGR);

            std::vector<cv::Point2f> centers;
            bool patternfound = cv::findCirclesGrid( undistortedIR, m_patternSize, centers, cv::CALIB_CB_ASYMMETRIC_GRID );

            if( patternfound )
            {
                cv::solvePnPRansac( m_corners, centers, m_camMat, m_distCoeffs, m_rvec, m_tvec, true );

                // calculate the rotation matrix and put it in the 4x4 transformation matrix
                cv::Matx33d rotMat;
                cv::Rodrigues( m_rvec, rotMat );
                for( int i = 0; i < 3; ++i )
                    for( int j = 0; j < 3; ++j )
                        transMat(i,j) = rotMat(i,j);

                // put the translation vector into the transformation matrix
                for( int i = 0; i < 3; ++i )
                    transMat(i,3) = m_tvec.at<double>(i);

                // DEBUG: project axes on the IR image plane and show them in the IR image
                std::vector<cv::Point2f> projectedPoints;
                std::vector<cv::Point3f> axis;
                const float l = m_patternSpacing * 3.f;
                axis.push_back(cv::Point3f(l,0,0));
                axis.push_back(cv::Point3f(0,l,0));
                axis.push_back(cv::Point3f(0,0,l));

                cv::projectPoints( axis, m_rvec, m_tvec, m_camMat, m_distCoeffs, projectedPoints );

                cv::line(viewColor, centers.at(0), projectedPoints.at(0), cv::Scalar(0,0,255), 2);
                cv::line(viewColor, centers.at(0), projectedPoints.at(1), cv::Scalar(0,255,0), 2);
                cv::line(viewColor, centers.at(0), projectedPoints.at(2), cv::Scalar(255,0,0), 2);
            }

            cv::imshow("Infrared and Pattern", viewColor);
        }
        else
            cv::imshow("Infrared and Pattern", undistortedIR);

        return transMat;
    });

    return camToPattern;
}

double PatternTracker::computeReprojectionErrors(const std::vector<std::vector<cv::Point3f> >& objectPoints,
                                                 const std::vector<std::vector<cv::Point2f> >& imagePoints,
                                                 const std::vector<cv::Mat>& rvecs, const std::vector<cv::Mat>& tvecs,
                                                 const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs,
                                                 std::vector<float>& perViewErrors )
{
    std::vector<cv::Point2f> imagePoints2;
    int i, totalPoints = 0;
    double totalErr = 0, err;
    perViewErrors.resize(objectPoints.size());

    for( i = 0; i < (int)objectPoints.size(); i++ )
    {
        projectPoints(cv::Mat(objectPoints[i]), rvecs[i], tvecs[i],
                      cameraMatrix, distCoeffs, imagePoints2);
        err = norm(cv::Mat(imagePoints[i]), cv::Mat(imagePoints2), cv::NORM_L2);
        int n = (int)objectPoints[i].size();
        perViewErrors[i] = (float)std::sqrt(err*err/n);
        totalErr += err*err;
        totalPoints += n;
    }

    return std::sqrt(totalErr/totalPoints);
}


