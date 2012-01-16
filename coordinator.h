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
    void stopFeed();

public slots:
    void controlOpenCvThread(bool startNew,QString dev="0");
    void LoadNewMovie(QString);
    void changeFps(int newFps);

private:
    MainGui* theGui;
    bool guiMode;
    PicBackend picBack;
    CamBackend camBack;

};

#endif // COORDINATOR_H
