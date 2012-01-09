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


    connect(fileDialog,SIGNAL(StaticPicPressed(QString)),this,SIGNAL(newPicNeeded(QString)));
//    connect(fileDialog,SIGNAL(OpencvFeedPressed()),this,SIGNAL(newOpencvFeedNeeded()));
    connect(fileDialog,SIGNAL(OpencvFeedPressed()),this,SLOT(startingOpenCVFeedTimer()));

    setScene(theScene);
}

void MainGui::resizeEvent(QResizeEvent *event)
{
    if (scene())
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
    QGraphicsView::resizeEvent(event);
}

void MainGui::newImageReceived(cv::Mat theMatrix)
{
    theScene->imageBuff=theMatrix.clone();
    theScene->update();
}

void MainGui::startingOpenCVFeedTimer()
{
    emit newOpencvFeedNeeded();
    QTimer::singleShot(20, this, SLOT(startingOpenCVFeedTimer()));
}
