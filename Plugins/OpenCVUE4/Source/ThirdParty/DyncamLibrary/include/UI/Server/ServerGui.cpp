//
// Created by ascadian on 8/1/17.
//

#include <QApplication>

#include "serverwindow.h"

class OpenCVCam;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    ServerWindow w;
    w.show();

    return a.exec();
}