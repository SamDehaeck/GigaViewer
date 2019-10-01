#ifndef INTERFEROPLUGIN_H
#define INTERFEROPLUGIN_H

#include <opencv2/opencv.hpp>
#include <QtCore>
#include "imagepacket.h"

class InterferoPlugin
{
private:
    int threshold;
    int minDiameter;
    int maxDiameter;
    bool activated;
    bool feedback;
    QStringList dataToSave;
public:
    InterferoPlugin(int thresh);
    void ChangeSettings(QMap<QString,QVariant> settings);
    bool processImage(ImagePacket& currIm);
};

#endif // InterferoPlugin_H
