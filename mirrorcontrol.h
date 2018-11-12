#ifndef MIRRORCONTROL_H
#define MIRRORCONTROL_H

#include <QThread>
#include <QtConcurrent/QtConcurrent>
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
    double Pos2Vx(double x, double y);
    double Pos2Vy(double x, double y);
    float coefX[10], coefY[10];

    float LasPatterns_X[10][100];
    float LasPatterns_Y[10][100];
    int LasPatterns_N[10];
    bool LasPatterns_act[10];
    bool LasPatterns_GenAct;
    int LasPatterns_Nact, LasPatterns_CurIndex;
    int LasPatterns_Time;

    void AdvanceCurrentIndex();
    void RunMirror_INT();

    QFuture<void> Mirror_future;
	
public:
    MirrorControl();
    bool Initialisation (int sampleRate);
	void Closing ();
    void ChangeMirrorStream (float x_vector[100], float y_vector[100], int Npoints);       //100 for 180°, 68 for 122.4° and 32 for 57.6°
    void ResetMirrorStream ();
    void ChangeMirrorPosition (float x, float y);
    void ChangeMirrorVoltage (float x, float y);
    void SetSampleRate (int rate);
    void ResetMirrorOffset ();
    void SetLasPattern(float xPos[], float yPos[], int NroPos, int Index, bool act);
    void SetActivate(bool act);
    void ResetLaserPatterns();

    void RunMirror_EXT();
};

#endif // MIRRORCONTROL_H
