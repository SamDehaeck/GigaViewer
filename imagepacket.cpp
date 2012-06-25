#include "imagepacket.h"

ImagePacket::ImagePacket()
{
}

ImagePacket::ImagePacket(const ImagePacket &src)
{
    image=src.image.clone();
    seqNumber=src.seqNumber;
    timeStamp=src.timeStamp;
}

ImagePacket::~ImagePacket()
{
}


