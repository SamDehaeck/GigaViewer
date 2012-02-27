#include "maingui.h"
#include "videoglscene.h"
#include "fileinputdialog.h"
#include "playbackdialog.h"
#include "cameracontrolsdialog.h"
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
    CameraControlsDialog* camDialog = new CameraControlsDialog;

    QList<QDialog*> controlDialogs;
    controlDialogs.append(camDialog);
    controlDialogs.append(playDialog);
    controlDialogs.append(fileDialog);


    theScene= new VideoGlScene(controlDialogs,parent);

    QShortcut *playToggle = new QShortcut(QKeySequence(Qt::Key_Space),this);
    connect(playToggle,SIGNAL(activated()),playDialog,SLOT(togglePlay()));
    QShortcut *reverse = new QShortcut(QKeySequence(Qt::Key_Left),this);
    connect(reverse,SIGNAL(activated()),playDialog,SLOT(reversePlay()));
    QShortcut *forward = new QShortcut(QKeySequence(Qt::Key_Right),this);
    connect(forward,SIGNAL(activated()),playDialog,SLOT(forwardPlay()));
    QShortcut * up = new QShortcut(QKeySequence(Qt::Key_Up),this);
    connect(up,SIGNAL(activated()),playDialog,SLOT(on_ffwdButton_clicked()));
    QShortcut * down = new QShortcut(QKeySequence(Qt::Key_Down),this);
    connect(down,SIGNAL(activated()),playDialog,SLOT(on_rwdButton_clicked()));
    QShortcut * stop = new QShortcut(QKeySequence(Qt::Key_Q),this);
    connect(stop,SIGNAL(activated()),playDialog,SLOT(on_stopButton_clicked()));


    connect(fileDialog,SIGNAL(StaticPicPressed(QString)),this,SIGNAL(newPicNeeded(QString)));
    connect(fileDialog,SIGNAL(MoviePressed(QString)),this,SLOT(newMoviePressed(QString)));
    connect(fileDialog,SIGNAL(OpencvFeedPressed()),this,SLOT(openCvFeedPressed()));
    connect(fileDialog,SIGNAL(AvtFeedPressed()),this,SLOT(AVTFeedPressed()));
    connect(fileDialog,SIGNAL(CloseApp()),this,SIGNAL(closeApplic()));
    connect(playDialog,SIGNAL(stopPlayback()),this,SLOT(stopButtonPressed()));
    connect(playDialog,SIGNAL(newFps(int)),this,SLOT(gotNewFps(int)));
    connect(playDialog,SIGNAL(recordNow(bool,QString,QString)),this,SIGNAL(startRecording(bool,QString,QString)));
    connect(camDialog,SIGNAL(NeedNewSample()),this,SLOT(needNewSample()));
    connect(this,SIGNAL(newSampleReady(ImagePacket)),camDialog,SLOT(GotNewSample(ImagePacket)));
    connect(camDialog,SIGNAL(SetShutterSpeed(int)),this,SIGNAL(setShutter(int)));
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
    if (getNewSample) {
        emit newSampleReady(theMatrix);
        getNewSample=FALSE;
    }
}

void MainGui::openCvFeedPressed()
{
    emit newOpencvFeedNeeded(TRUE);
    showPlaybackControls(TRUE);
    showInputControls(FALSE);
}

void MainGui::stopButtonPressed()
{
    emit newOpencvFeedNeeded(FALSE);
    showPlaybackControls(FALSE);
    showCameraControls(FALSE);
    showInputControls(TRUE);
}

void MainGui::gotNewFps(int fps)
{
    emit implementNewFps(fps);
}

void MainGui::newMoviePressed(QString theString)
{
    if (theString!="") {
        emit newMovieNeeded(theString);
        showPlaybackControls(TRUE);
        showInputControls(FALSE);
    }
}

void MainGui::showPlaybackControls(bool visible)
{
    foreach (QGraphicsItem *item,theScene->items()) {
        if (item->data(0)=="PLAYBACK") {
            item->setVisible(visible);
        }
    }
}

void MainGui::showInputControls(bool visible)
{
    foreach (QGraphicsItem *item,theScene->items()) {
        if (item->data(0)=="INPUT") {
            item->setVisible(visible);
        }
    }
}

void MainGui::showCameraControls(bool visible)
{
    foreach (QGraphicsItem *item,theScene->items()) {
        if (item->data(0)=="CAMERA") {
            item->setVisible(visible);
        }
    }
}

void MainGui::AVTFeedPressed()
{
    emit newAvtFeedNeeded(TRUE);
    showPlaybackControls(TRUE);
    showInputControls(FALSE);
    showCameraControls(TRUE);
}

void MainGui::needNewSample()
{
    getNewSample=TRUE;
}


