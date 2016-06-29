#ifndef MIRRORCONTROL_H
#define MIRRORCONTROL_H

#include <QtCore>

#include "MTIDefinitions.h"
#include "MTIDevice.h"
#include "MTIDataGenerator.h"

class MirrorControl
{
private:

    //CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//
    QString dataToSave;
    //CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//

    int vectorLength;
    MTIDevice * mti;
    unsigned char m_stream[100];                                             //100 for 180°, 68 for 122.4° and 32 for 57.6°
	unsigned char m_point;
    void readMappingParameters();
    float coefX[6], coefY[6];
	
public:
    MirrorControl();
    bool Initialisation ();
	void Closing ();
    void ChangeMirrorStream (float x_vector[100], float y_vector[100]);       //100 for 180°, 68 for 122.4° and 32 for 57.6°
    void ChangeMirrorPosition (float x, float y);
};

#endif // MIRRORCONTROL_H
