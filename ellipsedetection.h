#ifndef ELLIPSEDETECTION_H
#define ELLIPSEDETECTION_H

#include <opencv2/opencv.hpp>
#include <QtCore>
#include "imagepacket.h"

class EllipseDetection
{
private:
    int threshold;
    int targetX;
    int targetY;
    bool activated;
    bool shouldTrack;
    QStringList dataToSave;
public:
    EllipseDetection(int thresh);
    void ChangeSettings(QMap<QString,QVariant> settings);
    bool processImage(ImagePacket& currIm);
};

#endif // ELLIPSEDETECTION_H
