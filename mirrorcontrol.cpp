#include "mirrorcontrol.h"
#include <QDebug>

MirrorControl::MirrorControl()
{
    qDebug()<<"Creation of MirrorControl object";
	for( int i = 0; i < 100; i++ ){										//creation of the m vector used for dataStream
		m_stream[i]= 0;
	}
    m_point = 0;                                                        //creation of the m value used for the goToDevicePosition
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
	params->VdifferenceMax = 80;										//Device limitations
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
	
    mti->SendDataStream( x_vector, y_vector, m_stream, 100, 0, false );		// This call will download the buffer of data to the controller and run it when existing frame ends
}


//change mirror position
void MirrorControl::ChangeMirrorPosition (float x, float y){
	
	mti->GoToDevicePosition( x, y, m_point, 10 );						//This call will move the laser to new x, y position in 10ms
}
