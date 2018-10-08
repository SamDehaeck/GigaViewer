#include "mirrorcontrol.h"
#include <QDebug>
#include <QVariant>

#include <QList>
#include <QStringList>
#include <QDir>

MirrorControl::MirrorControl()
{
    qDebug()<<"Creation of MirrorControl object";
    vectorLength = 100;                                                 //100 for 180°, 68 for 122.4° and 32 for 57.6°
    for( int i = 0; i < vectorLength; i++ ){							//creation of the m vector used for dataStream
		m_stream[i]= 0;
	}
    m_point = 0;                                                        //creation of the m value used for the goToDevicePosition
    readMappingParameters();                                            //Reading and saving the parameters of the mirror/laser mapping
    for (int i=0; i<6; i++){
        qDebug() << "CoefX["+QString::number(i)+"]="+QString::number(coefX[i]);
    }
    for (int i=0; i<6; i++){
        qDebug() << "CoefY["+QString::number(i)+"]="+QString::number(coefY[i]);
    }

}

//Initialisation of the mirror
bool MirrorControl::Initialisation(int sampleRate){

    mti= new MTIDevice;                     							//Creation of a MTIDevice instance
	mti->ConnectDevice( "COM3" );										//Connection to the device	
	MTIError LastError;													//Verification of the proper connection with the COM
	LastError = mti->GetLastError();
	if (LastError != MTIError::MTI_SUCCESS){
		qDebug()<<"Problem with the COM port";
		mti->SendSerialReset();		
		return false;
	}
	MTIDeviceParams *params = mti->GetDeviceParams();					//Get current info and parameters from controller
    params->VdifferenceMax = 120;										//Device limitations
	params->HardwareFilterBw = 500;										//Device limitations
	params->DataMode = MTIDataMode::Sample_Output;						// Output mode sample (default)
	//params->DataScale = 1.0f;											// example of scaling data/content to 80%
	params->DeviceAxes = MTIAxes::Normal;								// Normal X,Y Axes Definition (default)
	params->SyncMode = MTISync::Output_DOut0;							// Default digital output levels at pin 0
    params->Vbias = 80;
    params->SampleRate = sampleRate;                                         //Points per second
	params->MEMSDriverEnable = true;									//Turn on the MEMS driver
	mti->SetDeviceParams( params );										//Setting all the parameters in the structure
	if (LastError != MTIError::MTI_SUCCESS){							//Verification of the proper reception of the parameters by the device
		qDebug()<<"Problem with the parameters";
		mti->ResetDevicePosition();										//Send analog outputs (and device) back to origin in 25ms
		mti->SetDeviceParam( MTIParam::MEMSDriverEnable, false );		//Turn off the MEMS driver
		mti->DisconnectDevice();
		mti->DeleteDevice();
		return false;
	}
	return true;
}

//Closing of the mirror
void MirrorControl::Closing(){

    MTIError LastError;

    mti->ResetDevicePosition(); 										//Send analog outputs (and device) back to origin in 25ms
    LastError = mti->GetLastError();
    qDebug()<<"Error on ResetDevicePosition: " <<LastError;

    mti->SetDeviceParam( MTIParam::MEMSDriverEnable, false );			//Turn off the MEMS driver
    LastError = mti->GetLastError();
    qDebug()<<"Error on turn off the MEMs driver: " <<LastError;

    mti->DisconnectDevice();
    LastError = mti->GetLastError();
    qDebug()<<"Error on DisconnectDevice: " <<LastError;

    mti->DeleteDevice();
    LastError = mti->GetLastError();
    qDebug()<<"Error on DeleteDevice: " <<LastError;

    delete mti;
}


//change mirror stream
void MirrorControl::ChangeMirrorStream (float x_vector[100], float y_vector[100]){
	
    //Mapping: conversion from Pixel to mirrorPosition (between -1 and +1)
    float xMirr[100], yMirr[100];                                                     //100 for 180°, 68 for 122.4° and 32 for 57.6°
    for (int i=0; i<vectorLength ; i++){
        xMirr[i] = Pos2Vx(x_vector[i],y_vector[i]);
        yMirr[i] = Pos2Vy(x_vector[i],y_vector[i]);
    }
    mti->SendDataStream( xMirr, yMirr, m_stream, vectorLength, 0, false );		// This call will download the buffer of data to the controller and run it when existing frame ends
}

void MirrorControl::ResetMirrorStream (){
    float xMirr, yMirr;
    xMirr = 0;
    yMirr = 0;
    mti->SendDataStream( &xMirr, &yMirr, m_stream, 1, 0, false );                                       // This call clear the buffer
}

//change mirror position
void MirrorControl::ChangeMirrorPosition (float x, float y){

    double data[1] = {0.0};
    double data2[1] = {0.0};
    float aux= 0.0;
    float aux2 = 0.0;

    data[0] =  Pos2Vx(x,y);
    data2[0] =  Pos2Vy(x,y);

    aux = float(data[0]);		aux2= float(data2[0]);

    mti->SetDeviceParam( MTIParam::OutputOffsets,    aux, aux2 );
}

void MirrorControl::ChangeMirrorVoltage (float x, float y){
    mti->SetDeviceParam( MTIParam::OutputOffsets,x,y );
}

void MirrorControl::SetSampleRate(int rate)
{
    mti->SetDeviceParam(MTIParam::SampleRate, rate);
}

void MirrorControl::ResetMirrorVoltage()
{
    mti->SetDeviceParam( MTIParam::OutputOffsets,0.0,0.0 );
}


void MirrorControl::readMappingParameters(){                                    //Used to load the mapping parameters

    QString filename = "D:/GitHub/SamDehaeck-GigaViewer/mappingXY.txt";
    QFile file (filename);
    QString line;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);

        for (int i=0; i<6; i++){
            line = in.readLine();
            coefX[i] = line.section('\t', 0,0).toFloat();
            coefY[i] = line.section('\t',1,1).toFloat();
        }
        file.close();
        qDebug() << "Mapping parameters correctly loaded";
    }
    else{
        qDebug() << "Problem with the reading of the mapping parameters in X";
        qDebug() << file.errorString();
    }
}

double MirrorControl::Pos2Vx(double x, double y){
    double Vx=0.0;
    Vx=coefX[0] + coefX[1]*x + coefX[2]*y + coefX[3]*x*x + coefX[4]*x*y + coefX[5]*y*y;
    //+ coefX[6]*x*x*x + coefX[7]*x*x*y + coefX[8]*x*y*y + coefX[9]*y*y*y;
    return Vx;
}

double MirrorControl::Pos2Vy(double x, double y){
    double Vy=0.0;
    Vy=coefY[0] + coefY[1]*x + coefY[2]*y + coefY[3]*x*x + coefY[4]*x*y + coefY[5]*y*y;
    //+ coefY[6]*x*x*x + coefY[7]*x*x*y + coefY[8]*x*y*y + coefY[9]*y*y*y;
    return Vy;
}
