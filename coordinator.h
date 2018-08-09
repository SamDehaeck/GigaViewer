#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <QObject>
#include "maingui.h"
#include "picbackend.h"
#include "cambackend.h"

class Coordinator : public QObject
{
    Q_OBJECT
public:
    Coordinator(MainGui* theGui, QObject *parent = 0);

signals:

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

private:
    MainGui* theGui;
    bool guiMode;
    PicBackend picBack;
    CamBackend camBack;
    bool opencvRunning;
    bool avtRunning;

};

#endif // COORDINATOR_H
