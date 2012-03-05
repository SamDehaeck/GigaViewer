#ifndef MAINGUI_H
#define MAINGUI_H

#include <QGraphicsView>
#include <QtGui>
#include <QGLWidget>
#include "imagepacket.h"
#include "videoglscene.h"
#include "cameracontrolsdialog.h"
#include "playbackdialog.h"
#include "fileinputdialog.h"



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
    void implementNewFps(int fps);
    void startRecording(bool start,QString recfold,QString codec);
    void closeApplic();
    void newSampleReady(ImagePacket matrix);
    void setShutter(int time);
    void setAutoShutter(bool fitRange);

public slots:
    void newImageReceived(ImagePacket theMatrix);
    void openCvFeedPressed();
    void AVTFeedPressed();
    void newMoviePressed(QString theString);
    void stopButtonPressed();
    void gotNewFps(int fps);
    void gotNewShutSpeed(int shut);
    void needNewSample();


protected:
    void resizeEvent(QResizeEvent *event);
    void showPlaybackControls(bool visible);
    void showInputControls(bool visible);
    void showCameraControls(bool visible);


private:
    VideoGlScene* theScene;
    FileInputDialog* fileDialog;
    PlaybackDialog* playDialog;
    CameraControlsDialog* camDialog;
    bool getNewSample;

};

#endif // MAINGUI_H
