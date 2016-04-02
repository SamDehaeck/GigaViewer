#ifndef MIRRORCONTROL_H
#define MIRRORCONTROL_H

#include "MTIDefinitions.h"
#include "MTIDevice.h"
#include "MTIDataGenerator.h"

//CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//
#include <QtCore>
//CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//

class MirrorControl
{
private:

    //CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//
    QString dataToSave;
    //CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//

    int vectorLength;
    MTIDevice * mti;
    unsigned char m_stream[100];
	unsigned char m_point;
    void readMappingParameters();
    float coefX[6], coefY[6];
	
public:
    MirrorControl();
    bool Initialisation ();
	void Closing ();
    void ChangeMirrorStream (float x_vector[100], float y_vector[100]);
    void ChangeMirrorPosition (float x, float y);
};

#endif // MIRRORCONTROL_H
