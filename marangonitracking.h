#ifndef MARANGONITRACKING_H
#define MARANGONITRACKING_H

#include <opencv2/opencv.hpp>
#include <QtCore>
#include "imagepacket.h"
#include "regulation.h"

#ifdef Q_OS_WIN32
    #include "mirrorcontrol.h"
#endif

class MarangoniTracking
{
private:
    int threshold;
    int nrParts;
    int targetX;
    int targetY;
    bool activated;

    void savingData ();
    QString dataToSave;

    int regulation_type;
    int target_type;

    float radius;

    float X_las_manual, Y_las_manual;

    Regulation myRegulator;

#ifdef Q_OS_WIN32
    MirrorControl mirCtrl;          //Mirror object used to access to the MirrorControl functions
#endif

    float Ppoint[2];
    std::vector<std::vector<cv::Point>> contoursP;
    std::vector<cv::Vec4i> hierachyP;

#ifdef Q_OS_WIN32
    bool initializeMirror();       //Used to initialize the MEM
    void closeMirror();            //Must be called when the the experiment is over to close the mirror
#endif

public:
    MarangoniTracking(int thresh,int nrParticles);
    void ChangeSettings(QMap<QString,QVariant> settings);
    bool processImage(ImagePacket& currIm);
    QMap<QString,QVariant> settingsBKUP;

};

#endif // MARANGONITRACKING_H
