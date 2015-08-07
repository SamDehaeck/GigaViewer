#include "vimbasourcesink.h"

using namespace AVT::VmbAPI;

VimbaSourceSink::VimbaSourceSink(): system ( AVT::VmbAPI::VimbaSystem::GetInstance() ),bufCount(50) {

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
                if (cameras.size()>1) {
                    for (uint i=0;i<cameras.size();i++) {
                        CameraPtr cam=cameras[i];
                        std::string namestr;
                        err=cam->GetName(namestr);
                        qDebug()<<"Next Camera is: "<<QString::fromStdString(namestr);
                    }
                }

                // now open the first one only
                pCamera=cameras[0];
                std::string camID;
                std::string namestr;
                err=pCamera->GetName(namestr);
                err=pCamera->GetID(camID);
                if( VmbErrorSuccess == err )    {
                    qDebug()<<"Opening camera "<<QString::fromStdString(namestr);

                    err=pCamera->Open(VmbAccessModeFull);
                    if (err==VmbErrorSuccess) {
                        //camera successfully opened. Now do some camera initialisation steps


                        // Set the GeV packet size to the highest possible value
                        // (In this example we do not test whether this cam actually is a GigE cam)
                        FeaturePtr pCommandFeature;
                        if ( VmbErrorSuccess == pCamera->GetFeatureByName( "GVSPAdjustPacketSize", pCommandFeature ))
                        {
                            if ( VmbErrorSuccess == pCommandFeature->RunCommand() )
                            {
                                bool bIsCommandDone = false;
                                do
                                {
                                    if ( VmbErrorSuccess != pCommandFeature->IsCommandDone( bIsCommandDone ))
                                    {
                                        break;
                                    }
                                } while ( false == bIsCommandDone );
                            }
                        }

                        // get/set some features
                        FeaturePtr pFeature;

                        // Check/set pixel format
                        // Set pixel format. For the sake of simplicity we only support Mono and BGR in this example.
                        err = pCamera->GetFeatureByName( "PixelFormat", pFeature );
                        if ( VmbErrorSuccess == err )
                        {
                            // Try to set BGR
                            err = pFeature->SetValue( VmbPixelFormatRgb8 );
                            if ( VmbErrorSuccess != err )
                            {
                                // Fall back to Mono
                                err = pFeature->SetValue( VmbPixelFormatMono8 );
                                qDebug()<<"Will work in Mono8 mode";
                            }

                        }

                        // Set Trigger source to fixedRate
                        err=pCamera->GetFeatureByName("TriggerSource",pFeature);
                        if (err==VmbErrorSuccess) {
                            pFeature->SetValue(5); //this should be fixedRate
                        }

                        // Set acquisition mode to continuous
                        err=pCamera->GetFeatureByName("AcquisitionMode",pFeature);
                        if (err==VmbErrorSuccess) {
                            pFeature->SetValue(0); // this should be continuous
                        }

                        // get height and width of current camera
                        err=pCamera->GetFeatureByName("Width",pFeature);
                        if (err==VmbErrorSuccess) {
                            pFeature->GetValue(width); // this should be continuous
                        }
                        err=pCamera->GetFeatureByName("Height",pFeature);
                        if (err==VmbErrorSuccess) {
                            pFeature->GetValue(height); // this should be continuous
                        }
                        qDebug()<<"Width and Height: "<<width<<", "<<height;

                        // make sure shutter time is manual
                        err=pCamera->GetFeatureByName("ExposureAuto",pFeature);
                        if (err==VmbErrorSuccess) {
                            pFeature->SetValue(0); // this should be manual exposure setting
                        }




                        // construct and connect frame observer to the camera
//                        frameWatcher=new FrameObserver( pCamera,  );


                    } else {
                        // camera did not open successfully
                        return false;
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
    SetShutter(100);
    SetInterval(100);
//    pCamera->StartContinuousImageAcquisition(bufCount,frameWatcher);
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
    FeaturePtr pFeature;
    double acqRate=1000.0/(1.0*msec);
    VmbErrorType err=pCamera->GetFeatureByName("AcquisitionFrameRateAbs",pFeature);
    if (err==VmbErrorSuccess) {
        err=pFeature->SetValue(acqRate); // this should be continuous
        if (err==VmbErrorSuccess) {
            frameRate=acqRate;
            qDebug()<<"New frame rate is: "<<frameRate;
            return true;
        }
    }
    qDebug()<<"Setting interval did not work: "<<err;
    return false;
}

bool VimbaSourceSink::SetShutter(int shutTime)
{
    FeaturePtr pFeature;
    double dShut=shutTime*1.0;
    VmbErrorType err=pCamera->GetFeatureByName("ExposureTimeAbs",pFeature);
    if (err==VmbErrorSuccess) {
        err=pFeature->SetValue(dShut);
        if (err==VmbErrorSuccess) {
            exposure=shutTime;
            qDebug()<<"New shutter time is: "<<dShut;
            return true;
        }
    }
    qDebug()<<"Setting shutter did not work: "<<err;
    return false;
}

int VimbaSourceSink::SetAutoShutter(bool fitRange)
{
    qDebug()<<"Vimba set auto shutter";
    return true;
}

