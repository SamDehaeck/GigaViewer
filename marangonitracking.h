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
    int threshold;
    int nrParts;
    int targetX;
    int targetY;
    bool activated;
    bool shouldTrack;

    void savingData ();
    QString dataToSave;

    int regulation_type;
    int target_type;
    int start_time;

    float radius;
#ifdef Q_OS_WIN32
    Regulation myRegulator;
#endif

    float Ppoint[2];
    std::vector<std::vector<cv::Point>> contoursP;
    std::vector<cv::Vec4i> hierachyP;
    void getPosPartLITTLE(cv::Mat src, std::vector<std::vector<cv::Point>> contoursf, cv::OutputArray hierarchy);

public:
    MarangoniTracking(int thresh,int nrParticles);
    void ChangeSettings(QMap<QString,QVariant> settings);
    bool processImage(ImagePacket& currIm);
};

#endif // MARANGONITRACKING_H
