#ifndef INTERFEROPLUGIN_H
#define INTERFEROPLUGIN_H

#include <opencv2/opencv.hpp>
#include <QtCore>
#include "imagepacket.h"

class InterferoPlugin
{
private:
    bool activated;
    bool feedback;

public:
    InterferoPlugin();
    void ChangeSettings(QMap<QString,QVariant> settings);
    bool processImage(ImagePacket& currIm);
};

#endif // InterferoPlugin_H
