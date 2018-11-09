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
    bool checkOrientation;
    int targetX;
    int targetY;
    bool activated;
    float timeStamp;

    bool firstScan;

    void savingData ();
    QString dataToSave;

    int regulation_type;
    int target_type;

    float radius;

    float X_las_manual, Y_las_manual, A_las_manual, R_pat_manual, Aux_pat_manual;
    int SampleRate, SampleSense;
    float Ident_dist, Ident_radius, Ident_aux;
    int Ident_dir;

    Regulation myRegulator;

#ifdef Q_OS_WIN32
    MirrorControl mirCtrl;          //Mirror object used to access to the MirrorControl functions
#endif

    float PpointX[10], PpointY[10];
    float PpointX_minus_1[10], PpointY_minus_1[10];
    float Pangle[10], Pangle_minus_1[10];
    std::vector<std::vector<cv::Point>> contoursP;
    std::vector<cv::Vec4i> hierachyP;
    bool orderPart();

    void analizeImage(ImagePacket& currIm);

    void drawLaser(cv::Mat outImage);
    void drawTarget(cv::Mat outImage);
    void drawParticles(cv::Mat outImage);
    void drawObstacle(cv::Mat outImage);
    void drawPath(cv::Mat outImage, bool closed);

    QString setdata();
    QString runMirrorCalibration();

#ifdef Q_OS_WIN32
    bool initializeMirror();       //Used to initialize the MEM
    void closeMirror();            //Must be called when the the experiment is over to close the mirror
#endif

    void ThreadsTest();

public:
    MarangoniTracking();
    void ChangeSettings(QMap<QString,QVariant> settings);
    bool processImage(ImagePacket& currIm);
    QMap<QString,QVariant> settingsBKUP;

};

#endif // MARANGONITRACKING_H
