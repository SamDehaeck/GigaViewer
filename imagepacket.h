#ifndef IMAGE_H
#define IMAGE_H

#include <QMetaType>
#include <opencv2/opencv.hpp>
#include  <stdint.h>  //for msvc10 compiler (to remove or make a condition for gcc

struct ImagePacket {
        cv::Mat image,mask,groupImage;
        std::string fileName;
        int seqNumber;
        double timeStamp;
        QString pixFormat;

        ImagePacket();
        ImagePacket(const ImagePacket &src);
        ~ImagePacket();
};

Q_DECLARE_METATYPE(ImagePacket);


#endif // IMAGE_H
