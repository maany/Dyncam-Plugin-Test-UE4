#include "serverwindow.h"
#include "ui_serverwindow.h"

#include <src/Reconstruction/CloudFromDepth.h>


ServerWindow::ServerWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::ServerWindow),
        m_pointsMapper(cam) {
    ui->setupUi(this);

    if (cam->Open()) {
        auto capabilities = cam->getStreamCapabilities();

        if (capabilities.find(BaseCam::StreamCapabilities::DEPTH) != capabilities.end()) {
            auto stretched = Transform(cam->DepthEventsSource, [](cv::Mat image) {
                // Histogram Equalization
                double min;
                double max;
                cv::minMaxIdx(image, &min, &max);
                cv::Mat adjMap = cv::Mat();
                // expand your range to 0..255. Similar to histEq();
                //(*image).convertTo(adjMap, CV_8UC1, 255 / (max-min), -255*min/(max-min)); // try this if the other method fails
                image.convertTo(adjMap, CV_8UC1, 255 / (max - min), -min);
                return adjMap;
            });
            m_depthObserver = Observe(stretched, [this](cv::Mat image) {
                ui->openCVviewer1->showImage(image);
            });

            // point cloud
            m_cloudObserver = Observe(
                    m_pointsMapper.PointPosEvents,
                    [this](const cv::Mat &points) {
                        ui->openCVviewer31->setPointPos(points);
                    });
        }

        if (capabilities.find(BaseCam::StreamCapabilities::IR) != capabilities.end()) {
            auto stretched = Transform(cam->IrEventsSource, [](cv::Mat image) {
                // Histogram Equalization
                double min;
                double max;
                cv::minMaxIdx(image, &min, &max);
                cv::Mat adjMap = cv::Mat();
                // expand your range to 0..255. Similar to histEq();
                //(*image).convertTo(adjMap, CV_8UC1, 255 / (max-min), -255*min/(max-min)); // try this if the other method fails
                image.convertTo(adjMap, CV_8UC1, 255 / (max - min), -min);
                return adjMap;
            });
            m_irObserver = Observe(stretched, [this](cv::Mat image) {
                ui->openCVviewer2->showImage(image);
            });
        }

        if (capabilities.find(BaseCam::StreamCapabilities::COLOR) != capabilities.end()) {
            m_colorObserver = Observe(cam->ColorEventsSource, [this](cv::Mat image) {
                ui->openCVviewer3->showImage(image);
            });

            // point cloud colors
            m_cloudObserver = Observe(
                    m_pointsMapper.PointColorsEvents,
                    [this](const cv::Mat &colors) {
                        ui->openCVviewer31->setPointColors(colors);
                    });
        }
    }
}

ServerWindow::~ServerWindow() {
    delete ui;
}