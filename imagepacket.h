#ifndef IMAGE_H
#define IMAGE_H

#include <QMetaType>
#include "opencv2/opencv.hpp"

struct ImagePacket {
        cv::Mat image,mask,groupImage;
        std::string fileName;
        int seqNumber;
        double timeStamp;

        ImagePacket();
        ImagePacket(const ImagePacket &src);
        ~ImagePacket();
};

Q_DECLARE_METATYPE(ImagePacket);


#endif // IMAGE_H
