//
// Created by ascadian on 8/1/17.
//

#include "qtopencvwidget.h"

std::mutex QTOpenCVWidget::drawMutex;

QTOpenCVWidget::QTOpenCVWidget(QWidget *parent) :
        QOpenGLWidget(parent) {
    mBgColor = QColor::fromRgb(150, 150, 150);
}


void QTOpenCVWidget::initializeGL() {
    //makeCurrent();
    initializeOpenGLFunctions();

    float r = ((float) mBgColor.darker().red()) / 255.0f;
    float g = ((float) mBgColor.darker().green()) / 255.0f;
    float b = ((float) mBgColor.darker().blue()) / 255.0f;
    glClearColor(r, g, b, 1.0f);
}


void QTOpenCVWidget::resizeGL(int width, int height) {
    //makeCurrent();
    glViewport(0, 0, (GLint) width, (GLint) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, -height, 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);

    recalculatePosition();

    Q_EMIT imageSizeChanged(mRenderWidth, mRenderHeight);

    updateScene();
}


void QTOpenCVWidget::updateScene() {
    if (this->isVisible()) update();
}

void QTOpenCVWidget::paintGL() {
    //makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderImage();
}


void QTOpenCVWidget::renderImage() {
    std::lock_guard<std::mutex> lock(drawMutex);
    //makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT);

    if (!mRenderQtImg.isNull()) {
        glLoadIdentity();

        glPushMatrix();
        {
            if (mResizedImg.width() <= 0) {
                if (mRenderWidth == mRenderQtImg.width() && mRenderHeight == mRenderQtImg.height())
                    mResizedImg = mRenderQtImg;
                else
                    mResizedImg = mRenderQtImg.scaled(QSize(mRenderWidth, mRenderHeight),
                                                      Qt::IgnoreAspectRatio,
                                                      Qt::SmoothTransformation);
            }

            // ---> Centering image in draw area
            glRasterPos2i(mRenderPosX, mRenderPosY);
            glPixelZoom(1, -1);
            glDrawPixels(mResizedImg.width(), mResizedImg.height(), GL_RGBA, GL_UNSIGNED_BYTE,
                         mResizedImg.bits());
        }
        glPopMatrix();

        // end
        glFlush();
    }
}


void QTOpenCVWidget::recalculatePosition() {
    mImgRatio = (float) mOrigImage.cols / (float) mOrigImage.rows;

    mRenderWidth = this->size().width();
    mRenderHeight = static_cast<int>(floor(mRenderWidth / mImgRatio));

    if (mRenderHeight > this->size().height()) {
        mRenderHeight = this->size().height();
        mRenderWidth = static_cast<int>(floor(mRenderHeight * mImgRatio));
    }

    mRenderPosX = static_cast<int>(floor((this->size().width() - mRenderWidth) / 2));
    mRenderPosY = static_cast<int>(-floor((this->size().height() - mRenderHeight) / 2));

    mResizedImg = QImage();
}


bool QTOpenCVWidget::showImage(const cv::Mat &image) {
    {
        std::lock_guard<std::mutex> lock(drawMutex);
        if (image.channels() == 3) {
            cvtColor(image, mOrigImage, CV_BGR2RGBA);
        } else if (image.channels() == 1) {
            cvtColor(image, mOrigImage, CV_GRAY2RGBA);
        } else if (image.channels() == 4) {
            image.copyTo(mOrigImage);
        } else {
            return false;
        }

        mRenderQtImg = QImage((const unsigned char *) (mOrigImage.data),
                              mOrigImage.cols, mOrigImage.rows,
                              mOrigImage.step1(), QImage::Format_RGB32);

        recalculatePosition();
    }
    updateScene();
    return true;
}