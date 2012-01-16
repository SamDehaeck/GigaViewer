#include "maingui.h"
#include "videoglscene.h"
#include "fileinputdialog.h"
#include "playbackdialog.h"
#include <QList>

MainGui::MainGui(QWidget *parent) :
    QGraphicsView(parent)
{
    setWindowTitle(tr("Gige Viewer"));
//    view.setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers))); //anti-aliased
    setViewport(new QGLWidget);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    FileInputDialog* fileDialog = new FileInputDialog;
    PlaybackDialog* playDialog = new PlaybackDialog;

    QList<QDialog*> controlDialogs;
    controlDialogs.append(playDialog);

    controlDialogs.append(fileDialog);

    theScene= new VideoGlScene(controlDialogs,parent);


    connect(fileDialog,SIGNAL(StaticPicPressed(QString)),this,SIGNAL(newPicNeeded(QString)));
    connect(fileDialog,SIGNAL(MoviePressed(QString)),this,SIGNAL(newMovieNeeded(QString)));
    connect(fileDialog,SIGNAL(OpencvFeedPressed()),this,SLOT(openCvFeedPressed()));
    connect(playDialog,SIGNAL(stopPlayback()),this,SLOT(stopButtonPressed()));
    connect(playDialog,SIGNAL(newFps(int)),this,SLOT(gotNewFps(int)));
    setScene(theScene);
}

void MainGui::resizeEvent(QResizeEvent *event)
{
    if (scene())
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
    QGraphicsView::resizeEvent(event);
}

void MainGui::newImageReceived(ImagePacket theMatrix)
{
    theScene->imageBuff=theMatrix.image;
    theScene->update();
}

void MainGui::openCvFeedPressed()
{
    foreach (QGraphicsItem *item,theScene->items()) {
        if (item->data(0)=="PLAYBACK") {
            if (!item->isVisible()) {
                item->setVisible(TRUE);
                emit newOpencvFeedNeeded(TRUE);
            }
        }
    }
}

void MainGui::stopButtonPressed()
{
    foreach (QGraphicsItem *item,theScene->items()) {
        if (item->data(0)=="PLAYBACK") {
            item->setVisible(FALSE);
            emit newOpencvFeedNeeded(FALSE);
        }
    }
}

void MainGui::gotNewFps(int fps)
{
    emit implementNewFps(fps);
}
