#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <src/Reconstruction/CloudFromDepth.h>
#include "src/ReactDomain.h"
#include "src/Cams/OpenCVCam/OpenCVCam.h"
#include "src/Cams/Freenect2/Freenect2Cam.h"

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow() override;

protected:
    ObserverT m_colorObserver, m_irObserver, m_depthObserver, m_cloudObserver;

    std::shared_ptr<BaseCam> cam = std::make_shared<freenect2cam::Freenect2Cam>();

    Ui::ServerWindow *ui;

    CloudFromDepth m_pointsMapper;
};

#endif // SERVERWINDOW_H
