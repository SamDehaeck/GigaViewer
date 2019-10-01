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
#include "coordinator.h"

#ifdef ELLIPSE
#include "ellipsedetectiondialog.h"
#endif

#ifdef INTERFERO
#include "interferoplugindialog.h"
#endif

class MainGui : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MainGui(Coordinator *boss,QWidget *parent = nullptr);
    void returnToStart();

signals:
    void newPicNeeded(QString theString);
    void newMovieNeeded(QString theString);
    void newOpencvFeedNeeded(bool start);
    void newAvtFeedNeeded(bool start);
    void newVimbaFeedNeeded(bool start);
    void newIdsFeedNeeded(bool start);
    void implementNewFps(double fps);
    void showNewFps(double msec);
    void startRecording(bool start,QString recfold,QString codec,int skipping);
    void closeApplic();
    void newSampleReady(ImagePacket matrix);
    void newFrameNrShowing(int nr);
    void setShutter(int time);
    void setAutoShutter(bool fitRange);
    void skipFrames(bool forward);
    void setRoiRows(int rows);
    void setRoiCols(int cols);

    void pluginSettingsChanged(QMap<QString,QVariant> settings);

public slots:
    void newImageReceived(ImagePacket theMatrix);
    void openCvFeedPressed();
    void AVTFeedPressed();
    void VimbaFeedPressed();
    void IdsFeedPressed();
    void newMoviePressed(QString theString);
    void stopButtonPressed();
    void gotNewFps(double fps);
    void gotNewFpsFromBackend(double fps);
    void gotNewShutSpeed(int shut);
    void needNewSample();
    void getSnapshot(QString location);
    void showPlaybackControls(bool visible);
    void showInputControls(bool visible);
    void showCameraControls(bool visible);


protected:
    void resizeEvent(QResizeEvent *event);

    void showPluginDialogs(bool visible);


private:
    Coordinator myBoss;
    VideoGlScene* theScene;
    FileInputDialog* fileDialog;
    PlaybackDialog* playDialog;
    CameraControlsDialog* camDialog;
    bool getNewSample;
    bool recordSnapshot;
    QString snapshotLocation;

    bool saveSnapshot(ImagePacket theImage);

#ifdef ELLIPSE
    EllipseDetectionDialog* ellipseDialog;
#endif
#ifdef INTERFERO
    InterferoPluginDialog* interferoDialog;
#endif
};

#endif // MAINGUI_H
