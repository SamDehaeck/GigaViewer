#include "vimbasourcesink.h"

using namespace AVT::VmbAPI;

VimbaSourceSink::VimbaSourceSink(CamBackend* par, QString formatstring): system ( AVT::VmbAPI::VimbaSystem::GetInstance() ),bufCount(50),initialStamp(0) {
    parent=par;
    format=formatstring;
}

bool VimbaSourceSink::IsOpened()
{
    return true;
}

bool VimbaSourceSink::Init()
{
    VmbErrorType    err=system.Startup();
    CameraPtrVector cameras;

    if( VmbErrorSuccess == err )
    {
        // now find all cameras
        err = system.GetCameras( cameras );            // Fetch all cameras known to Vimba
        if( VmbErrorSuccess == err )
        {

            if (cameras.size()>0) {
                if (cameras.size()>1) {
                    qDebug() << "Cameras found: " << cameras.size();
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
//                            VmbPixelFormatType theF;
//                            std::string form;
//                            err=pFeature->GetValue(form);
//                            listOptions(pFeature);


//                            qDebug()<<"Format is "<<QString::fromStdString(form);
                             //Remove the setting here for the moment to allow testing 12 bit mode
                             // Fall back to Mono
//                            err = pFeature->SetValue( VmbPixelFormatMono8 );
                            if (format=="MONO8") {
                                err = pFeature->SetValue( VmbPixelFormatMono8 );
                            } else if (format=="MONO12") {
                                err = pFeature->SetValue( VmbPixelFormatMono12 );
                            } else if (format=="BAYERRG8") {
                                err=pFeature->SetValue(VmbPixelFormatBayerRG8);
                            } else {
                                qDebug()<<"Pixel Format not recognised: "<<format;
                            }
                            if (err!=VmbErrorSuccess) {
                                qDebug()<<"Could not set requested pixel format.";
                            }
                            std::string form;
                            err=pFeature->GetValue(form);
                            qDebug()<<"Working in "<<QString::fromStdString(form)<<" mode";
//                                qDebug()<<"Will work in Mono8 mode";
                        }

                        // Set Trigger source to fixedRate
                        err=pCamera->GetFeatureByName("TriggerSource",pFeature);
                        if (err==VmbErrorSuccess) {
                            pFeature->SetValue("FixedRate");
                        }

                        // get Camera timestamp frequency
                        err=pCamera->GetFeatureByName("GevTimestampTickFrequency",pFeature);
                        if (err==VmbErrorSuccess) {
                            err=pFeature->GetValue(camFreq);
                            if (err==VmbErrorSuccess) {
                                //qDebug()<<"Camera freq is "<<(1.0*camFreq);
                            } else {
                                qDebug()<<"Could not extract freq: "<<err;
                            }
                        } else {
                            qDebug()<<"Could not query frequency: "<<err<<" => Will use LUT";
                            if (namestr=="GE1050") {
                                camFreq=79861111;
                            } else {
                                qDebug()<<"Model not yet in LUT => unreliable timestamps";
                            }
                        }


                        // Set acquisition mode to continuous
                        err=pCamera->GetFeatureByName("AcquisitionMode",pFeature);
                        if (err==VmbErrorSuccess) {
                            pFeature->SetValue("Continuous"); // this should be continuous
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

                        // make sure shutter time is manual
                        err=pCamera->GetFeatureByName("ExposureAuto",pFeature);
                        if (err==VmbErrorSuccess) {
                            pFeature->SetValue("Off"); // this should be manual exposure setting
                        }




                        // construct the frame observer to the camera
                        frameWatcher=new VimbaFrameObserver( pCamera, parent );

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
//    qDebug()<<"Vimba Start Acquisition";
    SetShutter(100);
    SetInterval(100);
    initialStamp=0; // to make sure I start counting time from zero
    pCamera->StartContinuousImageAcquisition(bufCount,IFrameObserverPtr(frameWatcher));
    return true;
}

bool VimbaSourceSink::StopAcquisition()
{
    VmbErrorType err;
    err=pCamera->StopContinuousImageAcquisition();
    if (err!=VmbErrorSuccess) {
        qDebug()<<"Stopping did not work: "<<err;
    }
    return true;
}

bool VimbaSourceSink::ReleaseCamera()
{
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
    if (indexIncrement<0) qDebug()<<"Cannot stream backwards";

    AVT::VmbAPI::FramePtr pFrame=frameWatcher->GetFrame();
    VmbErrorType err;
    VmbUint32_t width,height;
    VmbPixelFormatType pixFormat;
    err=pFrame->GetPixelFormat(pixFormat);
    err=pFrame->GetHeight(height);
    err=pFrame->GetWidth(width);
    if ((pixFormat==VmbPixelFormatMono8)||(pixFormat==VmbPixelFormatBayerRG8)) {
        target.image=cv::Mat(height,width,CV_8U);
        err=pFrame->GetImage(target.image.data); // assign the frame image buffer pointer to the target image
        if (err!=VmbErrorSuccess) {
            qDebug()<<"Something went wrong assigning the data";
        }
        if (pixFormat==VmbPixelFormatMono8) target.pixFormat="MONO8";
        if (pixFormat==VmbPixelFormatBayerRG8) target.pixFormat="BAYERRG8";
    } else if (pixFormat==VmbPixelFormatMono12) {
//        qDebug()<<"Help, 12 bit images coming in!!";
        target.image=cv::Mat(height,width,CV_16U);
        err=pFrame->GetImage(target.image.data);
        if (err!=VmbErrorSuccess) {
            qDebug()<<"Something went wrong with the reception of the 12 bit image: "<<err;
        } else {
            //qDebug()<<"12-bit image should be moving up the stack";
        }
        target.pixFormat="MONO12";

/*    } else if (pixFormat==VmbPixelFormatBayerRG8) {
        target.image=cv::Mat(height,width,CV_8UC3);
        cv::Mat dummy(height,width,CV_8U);
        err=pFrame->GetImage(dummy.data); // assign the frame image buffer pointer to the target image
        if (err!=VmbErrorSuccess) {
            qDebug()<<"Something went wrong assigning the colour data";
        }
        cv::cvtColor(dummy,target.image,CV_BayerRG2BGR);*/
    } else {
        qDebug()<<"Other pixel formats not yet working";
    }

    VmbUint64_t id;
    err=pFrame->GetFrameID(id);
    target.seqNumber=id;

    VmbUint64_t stamp;
    err=pFrame->GetTimestamp(stamp);
    if (initialStamp==0) {
        initialStamp=stamp;
        qint64 currMsec=QDateTime::currentMSecsSinceEpoch();
        timeOffset=currMsec;
    }
    target.timeStamp=timeOffset+1000.0*(stamp-initialStamp)/(1.0*camFreq);
//        qDebug()<<"Time Stamp in ms: "<<(target.timeStamp-timeOffset);

    pCamera->QueueFrame( pFrame ); // requeue here. Not sure what will happen if buffer too small!

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
//            qDebug()<<"New frame rate is: "<<frameRate;
            return true;
        } else {
            qDebug()<<"Setting fps did not work, will set it to the max possible: "<<err;
            err=pCamera->GetFeatureByName("AcquisitionFrameRateLimit",pFeature);
            double maxRate;
            if (err==VmbErrorSuccess) {
                err=pFeature->GetValue(maxRate);
                if (acqRate>maxRate) {
                    qDebug()<<"Trying to set freq beyond the current limit: "<<maxRate;
                    err=pCamera->GetFeatureByName("AcquisitionFrameRateAbs",pFeature);
                    acqRate=maxRate;
                    err=pFeature->SetValue(acqRate);
                    if (err!=VmbErrorSuccess) {
                        qDebug()<<"Setting to max also did not work!";
                        acqRate=maxRate-1;
                        err=pFeature->SetValue(acqRate);
                    }
                }
            }
        }
    }

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
//            qDebug()<<"New shutter time is: "<<dShut;
            return true;
        }
    }
    qDebug()<<"Setting shutter did not work: "<<err;
    return false;
}

int VimbaSourceSink::SetAutoShutter(bool fitRange)
{
//    qDebug()<<"Vimba set auto shutter";
    FeaturePtr pFeature;
    VmbErrorType err;
    double oldShut;
    err=pCamera->GetFeatureByName("ExposureTimeAbs",pFeature);
    if (err==VmbErrorSuccess) {
        pFeature->GetValue(oldShut);
    }

    err=pCamera->GetFeatureByName("ExposureAutoAlg",pFeature);
    if (err==VmbErrorSuccess) {

        if (fitRange) {
            err=pFeature->SetValue("FitRange"); // Fit Range algorithm
            if (err!=VmbErrorSuccess) qDebug()<<"Couldn't set Fit Range";
        } else {
            err=pFeature->SetValue("Mean"); // Mean algorithm
            if (err!=VmbErrorSuccess) qDebug()<<"Couldn't set Mean";
        }
    } else {
        qDebug()<<"Couldn't set the auto-algorithm";
    }

    err=pCamera->GetFeatureByName("ExposureAutoOutliers",pFeature);
    if (err==VmbErrorSuccess) {
        err=pFeature->SetValue(10); // 0.1% of the top pixels considered as outliers (important for fitRange algorithm!)
        if (err!=VmbErrorSuccess) qDebug()<<"Couldn't set num value for outliers";
    } else {
        qDebug()<<"Couldn't set the auto-outliers";
    }

    // now ready to do the once-off autosetting
    err=pCamera->GetFeatureByName("ExposureAuto",pFeature);
    if (err==VmbErrorSuccess) {
        err=pFeature->SetValue("Once");
        if (err!=VmbErrorSuccess) {
            qDebug()<<"Once did not work: "<<err;
        }
    } else {
        qDebug()<<"Did not find auto feature";
    }

    double newShut;
    err=pCamera->GetFeatureByName("ExposureTimeAbs",pFeature);
    if (err!=VmbErrorSuccess) {
        qDebug()<<"Could not lock exposureFeature";
    }

    err=pFeature->GetValue(newShut);
    if (err!=VmbErrorSuccess) {
        //should sleep a bit before reasking
        int counter=0;
        bool succ=false;
        while (not succ && (counter<10)) {
            Sleeper::msleep(300);
            err=pFeature->GetValue(newShut);
            if (err!=VmbErrorSuccess) {
                qDebug()<<"Could not get exposure value: "<<err;
            } else {
                succ=true;
            }
            counter++;
        }
    }

    return newShut;
}

void VimbaSourceSink::listOptions(FeaturePtr pFeature) {
    StringVector vals;
    pFeature->GetValues(vals);
    for (uint i=0;i<vals.size();i++) {
        qDebug()<<"Valid values: " << QString::fromStdString(vals[i]);
    }

}

