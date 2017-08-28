//
// Created by ascadian on 8/1/17.
//

#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_2_0>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <mutex>

class QTOpenCVWidget : public QOpenGLWidget, protected QOpenGLFunctions_2_0 {
Q_OBJECT
public:
    explicit QTOpenCVWidget(QWidget *parent = nullptr);

Q_SIGNALS:

    void imageSizeChanged(int outW, int outH); // Used to resize the image outside the widget

public Q_SLOTS:

    bool showImage(const cv::Mat &image); // Used to set the image to be viewed

protected:
    void initializeGL() override; // OpenGL initialization
    void paintGL() override; // OpenGL Rendering
    void resizeGL(int width, int height) override; // Widget Resize Event

    void updateScene();

    void renderImage();

private:

    QImage mRenderQtImg; // Qt image to be rendered
    QImage mResizedImg;
    cv::Mat mOrigImage; // original OpenCV image to be shown

    QColor mBgColor; // Background color

    float mImgRatio; // height/width ratio

    int mRenderWidth;
    int mRenderHeight;
    int mRenderPosX;
    int mRenderPosY;

    void recalculatePosition();

    static std::mutex drawMutex;
};

