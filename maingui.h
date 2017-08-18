#ifndef MAINGUI_H
#define MAINGUI_H

#include <QGraphicsView>
#include <QtGui>
#include <QOpenGLWidget>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
#include "imagepacket.h"
#include "videoglscene.h"
#include "cameracontrolsdialog.h"
#include "playbackdialog.h"
#include "fileinputdialog.h"

#ifdef TRACKING
#include "marangonitrackingdialog.h"
#endif

#ifdef ELLIPSE
#include "ellipsedetectiondialog.h"
#endif

class MainGui : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MainGui(QWidget *parent = 0);
    void returnToStart();

signals:
    void newPicNeeded(QString theString);
    void newMovieNeeded(QString theString);
    void newOpencvFeedNeeded(bool start);
    void newAvtFeedNeeded(bool start);
    void newVimbaFeedNeeded(bool start);
    void newIdsFeedNeeded(bool start);
    void implementNewFps(int fps);
    void showNewFps(int msec);
    void startRecording(bool start,QString recfold,QString codec,int skipping);
    void closeApplic();
    void newSampleReady(ImagePacket matrix);
    void newFrameNrShowing(int nr);
    void setShutter(int time);
    void setAutoShutter(bool fitRange);
    void skipFrames(bool forward);
    void setRoiRows(int rows);
    void setRoiCols(int cols);
#ifdef TRACKING
    void pluginSettingsChanged(QMap<QString,QVariant> settings);
#endif
#ifdef ELLIPSE
    void pluginSettingsChanged(QMap<QString,QVariant> settings);
#endif
public slots:
    void newImageReceived(ImagePacket theMatrix);
    void openCvFeedPressed();
    void AVTFeedPressed();
    void VimbaFeedPressed();
    void IdsFeedPressed();
    void newMoviePressed(QString theString);
    void stopButtonPressed();
    void gotNewFps(int fps);
    void gotNewFpsFromBackend(int fps);
    void gotNewShutSpeed(int shut);
    void needNewSample();
    void getSnapshot(QString location);


protected:
    void resizeEvent(QResizeEvent *event);
    void showPlaybackControls(bool visible);
    void showInputControls(bool visible);
    void showCameraControls(bool visible);

#ifdef TRACKING
    void showTrackingDialog(bool visible);
#endif
#ifdef ELLIPSE
    void showTrackingDialog(bool visible);
#endif

private:
    VideoGlScene* theScene;
    FileInputDialog* fileDialog;
    PlaybackDialog* playDialog;
    CameraControlsDialog* camDialog;
    bool getNewSample;
    bool recordSnapshot;
    QString snapshotLocation;

    bool saveSnapshot(ImagePacket theImage);

#ifdef TRACKING
    MarangoniTrackingDialog* trackDialog;
#endif
#ifdef ELLIPSE
    EllipseDetectionDialog* trackDialog;
#endif
};

#endif // MAINGUI_H
