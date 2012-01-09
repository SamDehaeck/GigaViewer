#include <QtGui>
#include <QGLWidget>

#include "maingui.h"
#include "coordinator.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainGui view;

    Coordinator theBoss(&view);






    view.showMaximized();

    return a.exec();
}
