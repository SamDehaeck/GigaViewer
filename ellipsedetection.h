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
    int targetAspectRatio;
    int targetAcceptableDist;
    int targetBWDistance;
    int targetValidationRatio;
    int targetDistanceCenterLine;
    int targetCenterLine;
    int targetVerticalLine;
    //double CenterLine;
    //double VerticalLine;
    bool UseAspectRatio;
    //bool UseContourLimits;
    bool UseBlackWhite;
    bool UseGeometryLine;
    bool UseDiamateterIntervalle;
    bool UseCoverage;

    bool activated;
    bool shouldTrack;
    QStringList dataToSave;
public:
    EllipseDetection(int thresh);
    void ChangeSettings(QMap<QString,QVariant> settings);
    bool processImage(ImagePacket& currIm);

};

cv::Point GetSamplePositions(cv::RotatedRect Ellipse,int i,int NbreEchantillon);
double ExtractValues(cv::Mat I,cv::Point Pixel);
cv::Point GetFocal1(cv::RotatedRect Ellipse);
cv::Point GetFocal2(cv::RotatedRect Ellipse);

#endif // ELLIPSEDETECTION_H
