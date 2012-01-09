#include "maingui.h"
#include "videoglscene.h"
#include "fileinputdialog.h"

MainGui::MainGui(QWidget *parent) :
    QGraphicsView(parent)
{
    setWindowTitle(tr("Gige Viewer"));
//    view.setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers))); //anti-aliased
    setViewport(new QGLWidget);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    FileInputDialog* fileDialog = new FileInputDialog;



    theScene= new VideoGlScene(fileDialog);


    connect(fileDialog,SIGNAL(buttonPressed(QString)),this,SIGNAL(newPicNeeded(QString)));

    setScene(theScene);
}

void MainGui::resizeEvent(QResizeEvent *event)
{
    if (scene())
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
    QGraphicsView::resizeEvent(event);
}

void MainGui::newPicReceived(cv::Mat theMatrix)
{
    theScene->imageBuff=theMatrix.clone();
    theScene->update();
}
