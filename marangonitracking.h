#ifndef MARANGONITRACKING_H
#define MARANGONITRACKING_H

#include <opencv2/opencv.hpp>
#include <QtCore>
#include "imagepacket.h"

#ifdef Q_OS_WIN32
    #include "regulation.h"
#endif
class MarangoniTracking
{
private:
    //CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//
    float center;
    //CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//
    int threshold;
    int nrParts;
    int targetX;
    int targetY;
    bool activated;
    bool shouldTrack;
    QStringList dataToSave;
#ifdef Q_OS_WIN32
    Regulation myRegulator;
#endif
public:
    MarangoniTracking(int thresh,int nrParticles);
    cv::Point FindParticle(cv::Mat image);
    void ChangeSettings(QMap<QString,QVariant> settings);
    bool processImage(ImagePacket& currIm);
};

#endif // MARANGONITRACKING_H
