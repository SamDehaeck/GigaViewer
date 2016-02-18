#ifndef MIRRORCONTROL_H
#define MIRRORCONTROL_H

#include "MTIDefinitions.h"
#include "MTIDevice.h"
#include "MTIDataGenerator.h"

class MirrorControl
{

private:
    MTIDevice * mti;
	unsigned char m_stream[100];
	unsigned char m_point;
	
public:
    MirrorControl();
    bool Initialisation ();
	void Closing ();
    void ChangeMirrorStream (float x_vector[100], float y_vector[100]);
    void ChangeMirrorPosition (float x, float y);
};

#endif // MIRRORCONTROL_H
