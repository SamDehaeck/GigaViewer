#ifndef PICBACKEND_H
#define PICBACKEND_H

#include <QObject>
#include "imagepacket.h"

class PicBackend : public QObject
{
    Q_OBJECT
public:
    explicit PicBackend(QObject *parent = 0);

signals:
    void NewImageReady(ImagePacket im);

public slots:
    void LoadNewImage(QString theName);

private:
    ImagePacket Image;

};

#endif // PICBACKEND_H
