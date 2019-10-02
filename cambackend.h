#ifndef CAMBACKEND_H
#define CAMBACKEND_H

#include <QObject>
#include <QtGui>
#include "imagepacket.h"
#include "imagesourcesink.h"
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
    void skipForwardBackward(bool forward);
    void StartRecording(bool start, QString recFold="", QString codec="",int skip=0);
    void changedPluginSettings(QMap<QString,QVariant> settings);
    void doPlugin(ImagePacket& currIm);


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


private:
    void run();
    void record();


    ImageSourceSink *currSink, *currSource;
    ImagePacket currImage;
    bool liveMode;
    bool recording;
    double timerInterval;
    QTimer* timer;
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
    QString recFileName;
    QString origin;



#ifdef ELLIPSE
    EllipseDetection ellipse;
#endif


};

#endif // CAMBACKEND_H
