#include "mirrorcontrol.h"

MirrorControl::MirrorControl()
{

}

bool MirrorControl::Connection() {
    MTIDevice * mti= new MTIDevice;

        //Connection to the device
        mti->ConnectDevice( "COM3" );
        //Verification of the proper connection with the COM
        MTIError LastError;
        LastError = mti->GetLastError();
        if (LastError != MTIError::MTI_SUCCESS){
        //    printf("Unable to connect with the port. Press any key to Exit.");
            mti->SendSerialReset();
            return false;
        }
        return true;
}

