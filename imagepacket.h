#ifndef IMAGE_H
#define IMAGE_H

#include <QMetaType>
#include <QString>
#include <opencv2/opencv.hpp>
#include <QVariantMap>
#include  <stdint.h>  //for msvc10 compiler (to remove or make a condition for gcc)

struct ImagePacket {
        cv::Mat image;//,mask,groupImage;
//        std::string fileName;
        int seqNumber;
        double timeStamp;
        QString pixFormat;
        QVariantMap message;

        ImagePacket();
        ImagePacket(const ImagePacket &src);
        ~ImagePacket();
};

Q_DECLARE_METATYPE(ImagePacket);


#endif // IMAGE_H
