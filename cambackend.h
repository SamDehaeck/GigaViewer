#ifndef CAMBACKEND_H
#define CAMBACKEND_H

#include <QObject>
#include <QtGui>
#include "imagepacket.h"
#include "imagesourcesink.h"
#ifdef TRACKING
#include "marangonitracking.h"
#endif
#ifdef ELLIPSE
#include "ellipsedetection.h"
#endif

class CamBackend : public QThread
{
    Q_OBJECT
public:
    explicit CamBackend(QObject *parent = nullptr);
    bool StartAcquisition(QString dev="0");
    void StopAcquisition();
    void ReleaseCamera();
    void SetInterval(double newInt);
    void SetShutter(int shut);
    void SetAutoShutter(bool fitRange);
    void setRoiRows(int rows);
    void setRoiCols(int cols);
    void AdaptForDisplay(ImagePacket& newIm);
    bool initProcPlugin();
    bool endProcPlugin();
    bool DoProcPlugin(ImagePacket& newIm);
    bool startRecPlugin(QString RecName);
    bool endRecPlugin();
    bool setSettingsPlugin(ImagePacket& newIm,QStringList settings);
    void skipForwardBackward(bool forward);
    void StartRecording(bool start, QString recFold="", QString codec="",int skip=0);

signals:
    void NewImageReady(ImagePacket im);
    void shutterChanged(int newTime);
    void fpsChanged(double msec);
    void startTheTimer(int interval);
    void stopTheTimer();

public slots:
    void GrabFrame();    
    void willStartTheTimer(int interval);
    void willStopTheTimer();

#ifdef TRACKING
    void changedPluginSettings(QMap<QString,QVariant> settings);
#endif
#ifdef ELLIPSE
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
    bool doPluginProcessing;
    int skipImages;
    int recSkip;
    bool stoppingRecording;


#ifdef TRACKING
    MarangoniTracking tracker;
#endif
#ifdef ELLIPSE
    EllipseDetection tracker;
#endif


};

#endif // CAMBACKEND_H
