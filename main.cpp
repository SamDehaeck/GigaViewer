#include <QtGui>
#include <QGLWidget>

#include "maingui.h"
#include "coordinator.h"
#include "imagepacket.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<ImagePacket>("ImagePacket");


    MainGui view;

    Coordinator theBoss(&view);






    view.showMaximized();

    return a.exec();
}
