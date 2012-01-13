#include <QtGui>
#include <QGLWidget>

#include "maingui.h"
#include "coordinator.h"
#include "imagepacket.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<ImagePacket>("ImagePacket");

    QMainWindow win;
    MainGui view;
    Coordinator theBoss(&view);
    win.setCentralWidget(&view);

    win.showMaximized();
    return a.exec();
}
