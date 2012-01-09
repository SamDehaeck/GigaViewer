#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <QObject>
#include "maingui.h"
#include "picbackend.h"

class Coordinator : public QObject
{
    Q_OBJECT
public:
    Coordinator(MainGui* theGui, QObject *parent = 0);

signals:

public slots:

private:
    MainGui* theGui;
    bool guiMode;
    PicBackend picBack;

};

#endif // COORDINATOR_H
