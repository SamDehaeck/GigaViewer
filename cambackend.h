#ifndef CAMBACKEND_H
#define CAMBACKEND_H

#include <QObject>
#include <QtGui>
#include "imagepacket.h"
#include "imagesourcesink.h"
#ifdef TRACKING
#include "marangonitracking.h"
#endif

class CamBackend : public QThread
{
    Q_OBJECT
public:
    explicit CamBackend(QObject *parent = 0);
    bool StartAcquisition(QString dev="0");
    void StopAcquisition();
    void ReleaseCamera();
    void SetInterval(int newInt);
    void SetShutter(int shut);
    void SetAutoShutter(bool fitRange);
    void AdaptForDisplay(ImagePacket& newIm);
    bool initProcPlugin();
    bool endProcPlugin();
    bool DoProcPlugin(ImagePacket& newIm);
    bool startRecPlugin(QString RecName);
    bool endRecPlugin();
    bool setSettingsPlugin(ImagePacket& newIm,QStringList settings);

signals:
    void NewImageReady(ImagePacket im);
    void shutterChanged(int newTime);
    void fpsChanged(int msec);
    void startTheTimer(int interval);
    void stopTheTimer();

public slots:
    void GrabFrame();
    void StartRecording(bool start, QString recFold="", QString codec="");
    void skipForwardBackward(bool forward);
    void willStartTheTimer(int interval);
    void willStopTheTimer();
    void setRoiRows(int rows);
    void setRoiCols(int cols);
#ifdef TRACKING
    void changedPluginSettings(QMap<QString,QVariant> settings);
#endif
private:
    void run();
    void record();


    ImageSourceSink *currSink, *currSource;
    ImagePacket currImage;
    bool liveMode;
    bool recording;
    double timerInterval;
    QTimer timer;
    bool reversePlay;
    bool isPaused;
    bool needTimer;
    bool doesCallBack;
    bool running;
    QString format;
#ifdef TRACKING
    MarangoniTracking tracker;
#endif
    bool doPluginProcessing;

};

#endif // CAMBACKEND_H
