#include "mirrorcontrol.h"
#include <QDebug>

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
}

//Initialisation of the mirror
bool MirrorControl::Initialisation(){

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
    params->SampleRate = 10000;
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
        xMirr[i] = coefX[0] + coefX[1]*x_vector[i] + coefX[2]*y_vector[i] + coefX[3]*x_vector[i]*x_vector[i] + coefX[4]*x_vector[i]*y_vector[i] + coefX[5]*y_vector[i]*y_vector[i];
        yMirr[i] = coefY[0] + coefY[1]*x_vector[i] + coefY[2]*y_vector[i] + coefY[3]*x_vector[i]*x_vector[i] + coefY[4]*x_vector[i]*y_vector[i] + coefY[5]*y_vector[i]*y_vector[i];
    }
    mti->SendDataStream( xMirr, yMirr, m_stream, vectorLength, 0, false );		// This call will download the buffer of data to the controller and run it when existing frame ends
}


//change mirror position
void MirrorControl::ChangeMirrorPosition (float x, float y){

    double data[1] = {0.0};
    double data2[1] = {0.0};
    float aux= 0.0;
    float aux2 = 0.0;

    //Coefficients defining the plane equation
    //double coefV1[6] = {4.15518970689798,	0.01587121536049,	-0.01218135670724,	0.00000086718561,	0.00000165423421,	0.00000002454538};
    double coefV1[10] = { -0.655000532732953,	0.001357002603236,	-0.000015826990825,	-0.000000119239143,	0.000000045650723,	-0.000000014553624,	0.000000000066090,	0.000000000021439,	0.000000000034664,	-0.000000000002177};


    //double coefV2[6] = {18.7896527377886,	-0.0174273406467,	-0.0119839925597,	0.0000014772564,	-0.0000011979468,	0.0000001710982},
    double coefV2[10]= { 0.49074276221346,	-0.00006661901633,	-0.00092062567618,	0.00000009139852,	0.00000002507538,	-0.00000006598950,	-0.00000000000854,	-0.00000000001583,	0.00000000000322,	0.0000000000453	};

    //On my new program I rely on a formula to do this, therefore I do not need so much detail
    data[0] =  coefV1[0] + coefV1[1]*x + coefV1[2]*y + coefV1[3]*x*x + coefV1[4]*x*y + coefV1[5]*y*y  + coefV1[6]*x*x*x+  coefV1[7]*y*x*x + coefV1[8]*x*y*y + coefV1[9]*y*y*y     ;
    data2[0] =  coefV2[0] + coefV2[1]*x + coefV2[2]*y + coefV2[3]*x*x + coefV2[4]*x*y + coefV2[5]*y*y  + coefV2[6]*x*x*x+  coefV2[7]*y*x*x + coefV2[8]*x*y*y + coefV2[9]*y*y*y     ;


    //diffticks = (clock() - tini)/(double(CLOCKS_PER_SEC));
    //cout<< fixed << setprecision(4) << diffticks <<endl;

    //printf(" Computed Voltages = %.2f , %.2f   \n", data[0], data2[0]) ;
    aux = float(data[0]);		aux2= float(data2[0]);

    //mti->GoToDevicePosition( aux, aux2, mData, mSec );
    mti->SetDeviceParam( MTIParam::OutputOffsets,    aux, aux2 );

    //dataNI[0] = float64(data[0]);
    //dataNI2[0] = float64( data2[0]);
    //DAQmxWriteAnalogF64(taskHandle, 1, 1,4.0,DAQmx_Val_GroupByChannel,dataNI,NULL,NULL);
    //DAQmxWriteAnalogF64(taskHandle2, 1, 1,4.0,DAQmx_Val_GroupByChannel,dataNI2,NULL,NULL);


    //OLD
    //Mapping: conversion from Pixel to mirrorPosition (between -1 and +1)
    //    float xMirr = coefX[0] + coefX[1]*x + coefX[2]*y + coefX[3]*x*x + coefX[4]*x*y + coefX[5]*y*y;
    //    float yMirr =  coefY[0] + coefY[1]*x + coefY[2]*y + coefY[3]*x*x + coefY[4]*x*y + coefY[5]*y*y;

    //    mti->GoToDevicePosition( xMirr, yMirr, m_point, 10 );						//This call will move the laser to new x, y position in 10ms
}


void MirrorControl::readMappingParameters(){                                    //Used to load the mapping parameters

    QString filenameX = "mappingX.txt";
    QFile fileX (filenameX);
    QString lineX;
    if(fileX.open(QIODevice::ReadOnly)){
        QTextStream in(&fileX);
        lineX = in.readLine();
        coefX[0] = lineX.section('\t', 0,0).toFloat();
        coefX[1] = lineX.section('\t',1,1).toFloat();
        coefX[2] = lineX.section('\t', 2,2).toFloat();
        lineX = in.readLine();
        coefX[3] = lineX.section('\t', 0,0).toFloat();
        coefX[4] = lineX.section('\t',1,1).toFloat();
        coefX[5] = lineX.section('\t', 2,2).toFloat();
        fileX.close();
        qDebug() << "Mapping parameters in X correctly loaded";
    }
    else{
        qDebug() << "Problem with the reading of the mapping parameters in X";
    }

    QString filenameY = "mappingY.txt";
    QFile fileY (filenameY);
    QString lineY;
    if(fileY.open(QIODevice::ReadOnly)){
        QTextStream in(&fileY);
        lineY = in.readLine();
        coefY[0] = lineY.section('\t', 0,0).toFloat();
        coefY[1] = lineY.section('\t',1,1).toFloat();
        coefY[2] = lineY.section('\t', 2,2).toFloat();
        lineY = in.readLine();
        coefY[3] = lineY.section('\t', 0,0).toFloat();
        coefY[4] = lineY.section('\t',1,1).toFloat();
        coefY[5] = lineY.section('\t', 2,2).toFloat();
        fileY.close();
        qDebug() << "Mapping parameters in Y correctly loaded";
    }
    else{
        qDebug() << "Problem with the reading of the mapping parameters in Y";
    }
}
