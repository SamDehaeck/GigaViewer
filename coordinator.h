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
    void startOpenCvThread(bool startNew);

private:
    MainGui* theGui;
    bool guiMode;
    PicBackend picBack;
    CamBackend camBack;

};

#endif // COORDINATOR_H
