#include "interferoplugin.h"


using namespace cv;

InterferoPlugin::InterferoPlugin() : activated(false),feedback(false){

}

void InterferoPlugin::ChangeSettings(QMap<QString,QVariant> settings) {
/*    minDiameter=settings["MinD"].toInt();
    maxDiameter=settings["MaxD"].toInt();
    threshold=settings["threshold"].toInt();
    if ((!activated)&&(settings["activated"].toBool())) {
        //qDebug()<<"Should initialise";
    }
    if (activated&&(!settings["activated"].toBool())) {
        //qDebug()<<"Should write to disk";
    }
*/
    activated=settings["activated"].toBool();
    feedback=settings["showFeedback"].toBool();
    //qDebug()<<"activated="<<activated;
}

bool InterferoPlugin::processImage(ImagePacket& currIm) {

    if (activated) {
        if (currIm.pixFormat=="RGB8") {
            cv::Mat grayIm;
            cv::cvtColor(currIm.image,grayIm,COLOR_RGB2GRAY);
            currIm.image=grayIm.clone();
            currIm.pixFormat="MONO8";  // will make it fall through to the next part.
        }
        if (currIm.pixFormat=="MONO8") {





        } else {
            qDebug()<<"Image format not yet supported! "<<currIm.pixFormat;
        }
    } else {
        currIm.message.insert("Interference Contrast",-1);  // will show a value for the fringe amplitude (Max-Min illumination)
    }
    return true;
}

