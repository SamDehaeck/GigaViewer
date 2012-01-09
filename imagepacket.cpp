#include "imagepacket.h"

ImagePacket::ImagePacket()
{
}

ImagePacket::ImagePacket(const ImagePacket &src)
{
    image=src.image.clone();
}

ImagePacket::~ImagePacket()
{
}


