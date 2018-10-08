#include "maingui.h"
#include <QList>

MainGui::MainGui(QWidget *parent) :
    QGraphicsView(parent)
{
    setWindowTitle(tr("Gige Viewer"));
//    view.setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers))); //anti-aliased
    setViewport(new QGLWidget);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    fileDialog = new FileInputDialog;
    playDialog = new PlaybackDialog;
    camDialog = new CameraControlsDialog;
#ifdef TRACKING
    trackDialog = new MarangoniTrackingDialog;
#endif


    QList<QDialog*> controlDialogs;
    controlDialogs.append(fileDialog);
    controlDialogs.append(playDialog);
    controlDialogs.append(camDialog);
#ifdef TRACKING
    controlDialogs.append(trackDialog);
#endif

//    const QSize rect =parent->frameSize();
//    qDebug()<<"Frame size: "<<rect.height()<<" "<<rect.width();
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
    connect(fileDialog,SIGNAL(VimbaFeedPressed()),this,SLOT(VimbaFeedPressed()));
    connect(fileDialog,SIGNAL(IdsFeedPressed()),this,SLOT(IdsFeedPressed()));
    connect(fileDialog,SIGNAL(CloseApp()),this,SIGNAL(closeApplic()));
    connect(playDialog,SIGNAL(stopPlayback()),this,SLOT(stopButtonPressed()));
    connect(playDialog,SIGNAL(newFps(int)),this,SLOT(gotNewFps(int)));
    connect(playDialog,SIGNAL(recordNow(bool,QString,QString)),this,SIGNAL(startRecording(bool,QString,QString)));
    connect(playDialog,SIGNAL(jumpFrames(bool)),this,SIGNAL(skipFrames(bool)));
    connect(camDialog,SIGNAL(NeedNewSample()),this,SLOT(needNewSample()));
    connect(this,SIGNAL(newSampleReady(ImagePacket)),camDialog,SLOT(GotNewSample(ImagePacket)));
    connect(camDialog,SIGNAL(SetShutterSpeed(int)),this,SIGNAL(setShutter(int)));
    connect(camDialog,SIGNAL(SetAutoShutter(bool)),this,SIGNAL(setAutoShutter(bool)));
    connect(this,SIGNAL(newFrameNrShowing(int)),playDialog,SLOT(newFrameNumberReceived(int)));
    connect(this,SIGNAL(showNewFps(int)),playDialog,SLOT(showNewFps(int)));
    connect(camDialog,SIGNAL(SetRoiRows(int)),this,SIGNAL(setRoiRows(int)));
    connect(camDialog,SIGNAL(SetRoiCols(int)),this,SIGNAL(setRoiCols(int)));
#ifdef TRACKING
    connect(trackDialog,SIGNAL(stateChanged(QMap<QString,QVariant>)),this,SIGNAL(pluginSettingsChanged(QMap<QString,QVariant>)));
    connect(theScene,SIGNAL(LclickOnImage(QPointF)),trackDialog,SLOT(LclickOnImageDetected(QPointF)));
    connect(theScene,SIGNAL(RclickOnImage(QPointF)),trackDialog,SLOT(RclickOnImageDetected(QPointF)));
#endif

    setScene(theScene);
    getNewSample=false;
}

void MainGui::returnToStart()
{
    showPlaybackControls(false);
    showCameraControls(false);
    showInputControls(true);
#ifdef TRACKING
    showTrackingDialog(false);
#endif
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
    emit newFrameNrShowing(theMatrix.seqNumber);
    if (getNewSample) {
        emit newSampleReady(theMatrix);
        getNewSample=false;
    }
}

void MainGui::openCvFeedPressed()
{
    showPlaybackControls(true);
#ifdef TRACKING
    showTrackingDialog(true);
#endif
    showInputControls(false);
    emit newOpencvFeedNeeded(true);
    this->parentWidget()->setWindowTitle("OpenCV Feed");
}

void MainGui::stopButtonPressed()
{
    emit newOpencvFeedNeeded(false);
    showPlaybackControls(false);
    showCameraControls(false);
    showInputControls(true);
#ifdef TRACKING
    showTrackingDialog(false);
#endif
    this->parentWidget()->setWindowTitle("GigaViewer");
}

void MainGui::gotNewFps(int fps)
{
    emit implementNewFps(fps);
}

void MainGui::gotNewFpsFromBackend(int fps)
{
    emit showNewFps(fps);
}

void MainGui::gotNewShutSpeed(int shut)
{
    camDialog->GotNewShutterSpeed(shut);
}

void MainGui::newMoviePressed(QString theString)
{
    if (theString!="") {
        emit newMovieNeeded(theString);
        showPlaybackControls(true);
        showInputControls(false);        
#ifdef TRACKING
        showTrackingDialog(true);
#endif
        this->parentWidget()->setWindowTitle(theString);
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
#ifdef TRACKING
void MainGui::showTrackingDialog(bool visible) {
    foreach (QGraphicsItem *item,theScene->items()) {
        if (item->data(0)=="PLUGIN") {
            item->setVisible(visible);
        }
    }
}
#endif
void MainGui::AVTFeedPressed()
{
    showPlaybackControls(true);
    showInputControls(false);
    showCameraControls(true);
#ifdef TRACKING
    showTrackingDialog(true);
#endif
    emit newAvtFeedNeeded(true);
    this->parentWidget()->setWindowTitle("AVT Live Camera Feed");
}

void MainGui::VimbaFeedPressed()
{
    emit newVimbaFeedNeeded(true);  // if this fails, the controls will be reset but this will be 'undone' by next commands
    showPlaybackControls(true);
    showInputControls(false);
    showCameraControls(true);
#ifdef TRACKING
    showTrackingDialog(true);
#endif
    this->parentWidget()->setWindowTitle("Vimba Live Camera Feed");
}

void MainGui::IdsFeedPressed()
{
    emit newIdsFeedNeeded(true);  // if this fails, the controls will be reset but this will be 'undone' by next commands
    showPlaybackControls(true);
    showInputControls(false);
    showCameraControls(true);
#ifdef TRACKING
    showTrackingDialog(true);
#endif
    this->parentWidget()->setWindowTitle("Ids Live Camera Feed");
}

void MainGui::needNewSample()
{
    getNewSample=true;
}



