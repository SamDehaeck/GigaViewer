#include "imagepacket.h"

ImagePacket::ImagePacket():pixFormat("")
{
}

ImagePacket::ImagePacket(const ImagePacket &src)
{
    image=src.image.clone();
    seqNumber=src.seqNumber;
    timeStamp=src.timeStamp;
    pixFormat=src.pixFormat;
}

ImagePacket::~ImagePacket()
{
}


