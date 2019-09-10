#include "imagepacket.h"

ImagePacket::ImagePacket():seqNumber(0),pixFormat(""),message()
{
}

ImagePacket::ImagePacket(const ImagePacket &src)
{
    image=src.image.clone();
//    fileName=src.fileName;
    seqNumber=src.seqNumber;
    timeStamp=src.timeStamp;
    pixFormat=src.pixFormat;
    message=src.message;
}

ImagePacket::~ImagePacket()
{
}


