#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <QObject>
#include <QThread>
//#include "maingui.h"
#include "picbackend.h"
#include "cambackend.h"

class Coordinator : public QObject
{
    Q_OBJECT
public:
    Coordinator(QObject *parent = nullptr);

signals:
    void NewImageReady(ImagePacket im);
    void shutterChanged(int newTime);
    void fpsChanged(double msec);

public slots:
    void controlCameraThread(bool startNew,QString dev="0");
    void LoadNewMovie(QString);
    void StartNewAVT(bool startNew);
    void StartNewVimba(bool startNew);
    void StartNewIds(bool startNew);
    void changeFps(double newFps);
    void stopAcquisition();
    void changeShutter (int time);
    void setAutoShutter (bool fitRange);
    void setRoiRows(int rows);
    void setRoiCols(int cols);
    void skipForwardBackward(bool forward);
    void StartRecording(bool start, QString recFold="", QString codec="",int skip=0);

public:
//    void setGui(MainGui* myGui);

private:
//    MainGui* theGui;
    bool guiMode;
    PicBackend picBack;
    CamBackend camBack;
    bool opencvRunning;
    bool avtRunning;
    QThread* backendThread;

};

#endif // COORDINATOR_H
