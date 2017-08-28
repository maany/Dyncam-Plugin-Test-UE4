//
// Created by ascadian on 8/1/17.
//

#pragma once

#include <mutex>
#include <atomic>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_2_0>
#include <opencv2/opencv.hpp>
#include <QtGui/QOpenGLShader>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QBasicTimer>

class PointsWidget : public QOpenGLWidget, protected QOpenGLFunctions_2_0 {
Q_OBJECT
public:
    explicit PointsWidget(QWidget* parent = nullptr);
    ~PointsWidget();

public
Q_SLOTS:
    void setPointPos(const cv::Mat &points);
	void setPointColors(const cv::Mat &colors);
	void setDirty();

protected:

    void initializeGL() override; /// OpenGL initialization
    void paintGL() override; /// OpenGL Rendering
    void resizeGL(int width, int height) override; /// Widget Resize Event

    void updateBuffers();

    void updateScene();

	void renderImage();

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

private:
    // continuous nx3 float, with n=number of points
    cv::Mat mPoints;
    // continuous nx4 float, with n=number of points, rgba order
    cv::Mat mColors;

    // Background color
    QColor mBgColor{200, 200, 200};

    // camera
    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    float angularSpeed;
    QQuaternion rotation;

	QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};
    int m_vboBuffSize = 6078464; // currently fits kinect TODO: make dynamic
    QMatrix4x4 mProjection;
    QOpenGLShaderProgram mShaderProgram;
    // attribute location in shader
    int m_vertexLocation;
    int m_mvpLocation;
    int m_colortLocation;
    int m_pointSizeLocation;

	static std::mutex drawMutex;
	std::atomic<bool> m_updateBuffers{false};
    QBasicTimer timer;

};

