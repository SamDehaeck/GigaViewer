#ifndef MARANGONITRACKING_H
#define MARANGONITRACKING_H

#include <opencv2/opencv.hpp>
#include <QtCore>
#include "imagepacket.h"

#include "mirrorcontrol.h"

class MarangoniTracking
{
private:
    int threshold;
    int nrParts;
    int targetX;
    int targetY;
    bool activated;
    bool shouldTrack;
    QStringList dataToSave;
    MirrorControl mirCtrl;
public:
    MarangoniTracking(int thresh,int nrParticles);
    cv::Point FindParticle(cv::Mat image);
    void ChangeSettings(QMap<QString,QVariant> settings);
    bool processImage(ImagePacket& currIm);
};

#endif // MARANGONITRACKING_H
