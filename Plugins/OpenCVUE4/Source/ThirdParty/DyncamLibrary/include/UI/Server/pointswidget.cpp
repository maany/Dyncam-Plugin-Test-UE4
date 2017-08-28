//
// Created by ascadian on 8/1/17.
//

#include "pointswidget.h"
#include <QMouseEvent>
#include <QOpenGLDebugLogger>

std::mutex PointsWidget::drawMutex;


PointsWidget::PointsWidget(QWidget *parent) :
        QOpenGLWidget(parent) {

    // sample points just to show something without camera
    mPoints.create(4, 3, CV_32F);
    mPoints.at<float>(0,0) = .1;
    mPoints.at<float>(0,1) = .1;
    mPoints.at<float>(0,2) = .1;

    mPoints.at<float>(1,0) = 100;
    mPoints.at<float>(1,1) = 100;
    mPoints.at<float>(1,2) = 100;

    mPoints.at<float>(2,0) = 1;
    mPoints.at<float>(2,1) = 0;
    mPoints.at<float>(2,2) = 1;

    mPoints.at<float>(3,0) = 1;
    mPoints.at<float>(3,1) = 0;
    mPoints.at<float>(3,2) = 0;

    mColors.create(4, 4, CV_32F);
    mColors.at<float>(0,0) = 0;
    mColors.at<float>(0,1) = 0;
    mColors.at<float>(0,2) = 1;
    mColors.at<float>(0,3) = 1;

    mColors.at<float>(1,0) = 1;
    mColors.at<float>(1,1) = 1;
    mColors.at<float>(1,2) = 1;
    mColors.at<float>(1,3) = 1;

    mColors.at<float>(2,0) = 0;
    mColors.at<float>(2,1) = 1;
    mColors.at<float>(2,2) = 0;
    mColors.at<float>(2,3) = 1;

    mColors.at<float>(3,0) = 1;
    mColors.at<float>(3,1) = 0;
    mColors.at<float>(3,2) = 0;
    mColors.at<float>(3,3) = 1;
}

PointsWidget::~PointsWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    m_vbo.destroy();
    doneCurrent();
}

void PointsWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void PointsWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}


void PointsWidget::timerEvent(QTimerEvent *e)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Request an update
        update();
    }
}


void PointsWidget::initializeGL() {

    QOpenGLContext *ctx = QOpenGLContext::currentContext();
    if (ctx->hasExtension(QByteArrayLiteral("GL_KHR_debug"))) {
        QOpenGLDebugLogger *logger = new QOpenGLDebugLogger(this);
        logger->initialize(); // initializes in the current context, i.e. ctx
        connect(logger, &QOpenGLDebugLogger::messageLogged, this, [] (const QOpenGLDebugMessage &debugMessage) {
            GLenum source = debugMessage.source();
            GLenum type = debugMessage.type();
            GLuint id = debugMessage.id();
            GLenum severity = debugMessage.severity();
            QString message = debugMessage.message();

            // ignore non-significant error/warning codes
            if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

            std::cout << "---------------" << std::endl;
            std::cout << "Debug message (" << id << "): " <<  message.toUtf8().constData() << std::endl;

            switch (source)
            {
                case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
                case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
                case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
                case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
                case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
            } std::cout << std::endl;

            switch (type)
            {
                case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
                case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
                case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
                case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
                case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
                case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
                case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
            } std::cout << std::endl;

            switch (severity)
            {
                case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
                case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
                case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
                case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
            } std::cout << std::endl;
            std::cout << std::endl;
        });
        logger->startLogging();
    } else {
        std::cout << "Logging is not supported by your GPU." << std::endl;
    }


    initializeOpenGLFunctions();

    float r = ((float) mBgColor.darker().red()) / 255.0f;
    float g = ((float) mBgColor.darker().green()) / 255.0f;
    float b = ((float) mBgColor.darker().blue()) / 255.0f;
    glClearColor(r, g, b, 1.0f);

    mShaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex,
                                           "attribute highp vec4 posAttr;\n"
										   "attribute lowp vec4 colAttr;\n"
										   "varying lowp vec4 col;\n"
										   "uniform highp mat4 matrix;\n"
                                           "uniform float pointSizeUniform;\n"
										   "void main() {\n"
                                           //"    col = vec4(1.0,1.0,1.0,1.0);\n"
                                           //"    col = vec4(pointSizeUniform,pointSizeUniform,pointSizeUniform,pointSizeUniform);\n"
                                           "    col = colAttr;\n"
                                           "    float p_size = pointSizeUniform;\n"
                                           "    gl_PointSize = clamp(p_size, 3.0, 20.0); \n"
										   "    gl_Position = matrix * posAttr;\n"
										   "}\n");


    mShaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment,
                                           "varying lowp vec4 col;\n"
										   "void main() {\n"
										   "   gl_FragColor = col;\n"
										   "}\n");
	
    if (!mShaderProgram.link()) {
        std::cerr << mShaderProgram.log().toStdString() << std::endl;
    }

    // location of shader attributes
    m_vertexLocation = mShaderProgram.attributeLocation("posAttr");
    m_mvpLocation = mShaderProgram.uniformLocation("matrix");
    m_colortLocation = mShaderProgram.attributeLocation("colAttr");
    m_pointSizeLocation = mShaderProgram.attributeLocation("pointSizeUniform");

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    // variable point size
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}


void PointsWidget::resizeGL(int width, int height) {
     // Calculate aspect ratio
    float aspect = width / static_cast<float>(height != 0 ? height : 1);

    // Set perspective projection
    const float zNear = 0.01, zFar = 7.0, fov = 45.0;
    mProjection.setToIdentity();
    mProjection.perspective(fov, aspect, zNear, zFar);

    // create buffers
    m_vbo.create();
    m_vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_vbo.bind();
    m_vbo.allocate(m_vboBuffSize * sizeof(float));
    m_vbo.release();

    // init buffers
    m_updateBuffers.store(true);

    updateScene();
}


void PointsWidget::updateScene() {
    if (this->isVisible()) {
        update();
    }
}

void PointsWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderImage();
}


void PointsWidget::renderImage() {
    std::lock_guard<std::mutex> lock(drawMutex);
    // update buffers if needed
    updateBuffers();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_vbo.bind();
    mShaderProgram.bind();

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -3.0f);
    matrix.rotate(rotation);
    // Set modelview-projection matrix
    mShaderProgram.setUniformValue(m_mvpLocation, mProjection * matrix);

    // this is broken. I can't get the uniform into the shader
    //float pointSize = 1.0f;
    //mShaderProgram.setUniformValue(m_pointSizeLocation, pointSize);
    //glUniform1f(m_pointSizeLocation, pointSize);
    //mShaderProgram.setUniformValueArray(m_pointSizeLocation, &pointSize, 1, 1);
    //glUniform1fv(m_pointSizeLocation, 1, &pointSize);

    // Tell OpenGL programmable pipeline how to locate vertex position data
    mShaderProgram.enableAttributeArray(m_vertexLocation);
    mShaderProgram.setAttributeBuffer(m_vertexLocation, GL_FLOAT, 0, 3);
    // point color data
    mShaderProgram.enableAttributeArray(m_colortLocation);
    const int colorOffset = static_cast<int>(mPoints.total() * mPoints.elemSize());
    mShaderProgram.setAttributeBuffer(m_colortLocation, GL_FLOAT, colorOffset, 4);

    // draw points
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(mPoints.rows));

    m_vbo.release();
    glFlush();
}


void PointsWidget::setPointPos(const cv::Mat &points) {
    mPoints = points;
    setDirty();
    updateScene();
}


void PointsWidget::setPointColors(const cv::Mat &colors) {
    mColors = colors;
    setDirty();
    updateScene();
}


void PointsWidget::setDirty() {
    m_updateBuffers.store(true);
}


void PointsWidget::updateBuffers() {
    if(!m_updateBuffers.load())
        return;
    // we assume the lock is active and we are on the opengl thread
    m_updateBuffers.store(false);

    if (mPoints.rows != mColors.rows) {
        // FIXME: here are dragons. colors and point positions should always be the same size.
        // due to human error or multithreading this might not allways be true
        std::cerr << "Point and color buffer size mismatch. Did you set them indepenently?" << std::endl;
    }

    // copy points into vbo
    m_vbo.bind();

    // possible optimization:
//    //auto ptr = m_vbo.mapRange(0, m_vboBuffSize, QOpenGLBuffer::RangeInvalidateBuffer | QOpenGLBuffer::RangeWrite);
//    auto ptr = m_vbo.map(QOpenGLBuffer::WriteOnly);
//    memcpy(ptr, mPoints.data(),  mPoints.size() * sizeof(mPoints[0]));
//    m_vbo.unmap();
    assert(mPoints.isContinuous()); //http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat
    assert(mPoints.elemSize() == sizeof(GL_FLOAT));
    assert(mColors.isContinuous()); //http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat
    assert(mColors.elemSize() == sizeof(GL_FLOAT));

    const int pointMemSize = static_cast<int>(mPoints.total() * mPoints.elemSize());
    m_vbo.write(0, mPoints.data, pointMemSize);

    // colors
    auto colorMemSize = static_cast<const int>(mColors.total() * mColors.elemSize());
    m_vbo.write(pointMemSize, mColors.data, colorMemSize);

    assert(m_vboBuffSize >= pointMemSize + colorMemSize);

    m_vbo.release();
}
