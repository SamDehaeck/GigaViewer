#include "vimbasourcesink.h"

using namespace AVT::VmbAPI;

VimbaSourceSink::VimbaSourceSink(): system ( AVT::VmbAPI::VimbaSystem::GetInstance() ) {

}

bool VimbaSourceSink::IsOpened()
{
    return true;
}

bool VimbaSourceSink::Init()
{
    qDebug()<<"Vimba Init";
    VmbErrorType    err=system.Startup();
    CameraPtrVector cameras;

    if( VmbErrorSuccess == err )
    {
        // now find all cameras
        err = system.GetCameras( cameras );            // Fetch all cameras known to Vimba
        if( VmbErrorSuccess == err )
        {
            qDebug() << "Cameras found: " << cameras.size();
            if (cameras.size()>0) {
                for (uint i=0;i<cameras.size();i++) {
                    CameraPtr cam=cameras[i];
                    std::string namestr;
                    err=cam->GetName(namestr);
                    qDebug()<<"Next Camera is: "<<QString::fromStdString(namestr);
                }

                // now open the first one only
                std::string camID;
                std::string namestr;
                err=cameras[0]->GetName(namestr);
                err=cameras[0]->GetID(camID);
                if( VmbErrorSuccess == err )    {
                    qDebug()<<"Opening camera "<<QString::fromStdString(namestr);
                    err=system.OpenCameraByID(camID.c_str(),VmbAccessModeFull,pCamera);
                    if (err==VmbErrorSuccess) {
                        qDebug()<<"Succesfully opened it";

                    }
                }
            }



        }
        else
        {
            qDebug() << "Could not list cameras. Error code: " << err;
        }
        return true;
    }
    else
    {
        qDebug() << "Could not start system. Error code: " << err;
        return false;
    }



}

bool VimbaSourceSink::StartAcquisition(QString dev)
{
    if (dev!="Vimba") {
        qDebug()<<"Device name incorrect for Vimba: "<<dev;
    }
    qDebug()<<"Vimba Start Acquisition";
    return true;
}

bool VimbaSourceSink::StopAcquisition()
{
    qDebug()<<"Vimba Stop Acquisition";
    return true;
}

bool VimbaSourceSink::ReleaseCamera()
{
    qDebug()<<"Vimba Release Camera";
    VmbErrorType err=pCamera->Close();
    if (err!=VmbErrorSuccess) {
        qDebug()<<"Problem closing the camera";
    }
    err=system.Shutdown();
    if (err!=VmbErrorSuccess) {
        qDebug()<<"Problem shutting down Vimba";
    }
    return true;
}

bool VimbaSourceSink::GrabFrame(ImagePacket &target, int indexIncrement)
{
//    qDebug()<<"VimbaSourceSink Grab Frame";
    return true;

}

bool VimbaSourceSink::SetInterval(int msec)
{
    qDebug()<<"Vimba Set Interval";
    return true;
}

bool VimbaSourceSink::SetShutter(int shutTime)
{
    qDebug()<<"Vimba Set Shutter";
    return true;
}

int VimbaSourceSink::SetAutoShutter(bool fitRange)
{
    qDebug()<<"Vimba set auto shutter";
    return true;
}

