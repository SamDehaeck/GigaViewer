#include "vimbasourcesink.h"
#include <QInputDialog>

using namespace AVT::VmbAPI;

VimbaSourceSink::VimbaSourceSink(CamBackend* par): system ( AVT::VmbAPI::VimbaSystem::GetInstance() ),bufCount(50),initialStamp(0) {
    parent=par;
}

void VimbaSourceSink::setFormat(QString formatstring) {
    format=formatstring;
    FeaturePtr pFeature;
    VmbErrorType err;

    // Check/set pixel format
    // Set pixel format. For the sake of simplicity we only support Mono and BGR in this example.
    err = pCamera->GetFeatureByName( "PixelFormat", pFeature );
    if ( VmbErrorSuccess == err )
    {
        if (format=="MONO8") {
            err = pFeature->SetValue( VmbPixelFormatMono8 );
        } else if (format=="MONO12") {
            err = pFeature->SetValue( VmbPixelFormatMono12 );
        } else if (format=="MONO14") {
            err = pFeature->SetValue( VmbPixelFormatMono14 );
        } else if (format=="BAYERRG8") {
            err=pFeature->SetValue(VmbPixelFormatBayerRG8);
        } else if (format=="BAYERGB8") {
            err=pFeature->SetValue(VmbPixelFormatBayerGB8);
        } else if (format=="BAYERRG12") {
            err=pFeature->SetValue(VmbPixelFormatBayerRG12);
        } else if (format=="RGB8Packed") {
            err=pFeature->SetValue(VmbPixelFormatRgb8);
        } else {
            qDebug()<<"Pixel Format not yet working in Gigaviewer: "<<format;
        }
        if (err!=VmbErrorSuccess) {
            qDebug()<<"Could not set requested pixel format.";
        }
        std::string form;
        err=pFeature->GetValue(form);
//        qDebug()<<"Working in "<<QString::fromStdString(form)<<" mode";
//                                qDebug()<<"Will work in Mono8 mode";
    }
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
        if( VmbErrorSuccess == err ) {

            bool camIsOpen=false;
            if (cameras.size()==0) {
                CameraPtr ipCam;
                if ( VmbErrorSuccess == system.OpenCameraByID("169.254.1.55",VmbAccessModeFull,ipCam)) {
                     qDebug()<<"IP camera opened";
                     cameras.push_back(ipCam);
                     camIsOpen=true;
                }

            }

            if (cameras.size()>0) {
                if (cameras.size()>1) {
                    QStringList cams;
                    qDebug() << "Cameras found: " << cameras.size();  // should also implement Qinputdialog to let the user choose which one to use
                    for (uint i=0;i<cameras.size();i++) {
                        CameraPtr cam=cameras[i];
                        std::string namestr;
                        err=cam->GetName(namestr);
                        cams<<QString::fromStdString(namestr);
                        qDebug()<<"Next Camera is: "<<QString::fromStdString(namestr);
                    }

                    bool camok;
                    QString theOne = QInputDialog::getItem(NULL, "Multiple cameras present",
                                                        "Selection options:", cams, 0, false, &camok);
                    if (camok && !theOne.isEmpty()) {
                        int index = cams.indexOf(theOne);
                        pCamera=cameras[index];
                    }

                } else {
                    // now open the first one only
                    pCamera=cameras[0];
                }


                std::string camID;
                std::string namestr;
                err=pCamera->GetName(namestr);
                err=pCamera->GetID(camID);
                if( VmbErrorSuccess == err )    {
                    qDebug()<<"Opening camera "<<QString::fromStdString(namestr);

                    if (!camIsOpen) err=pCamera->Open(VmbAccessModeFull);
                    if (err==VmbErrorSuccess) {
                        //camera successfully opened. Now do some camera initialisation steps


                        // Set the GeV packet size to the highest possible value => will make JAI camera stop to work correctly!
                        // (In this example we do not test whether this cam actually is a GigE cam)
                        FeaturePtr pCommandFeature;
/*                        if ( VmbErrorSuccess == pCamera->GetFeatureByName( "GVSPAdjustPacketSize", pCommandFeature ))
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
*/
                        // get/set some features
                        FeaturePtr pFeature;

                        // Set Trigger source to fixedRate if it exists (only for AVT cameras)
                        err=pCamera->GetFeatureByName("TriggerSource",pFeature);
                        if (err==VmbErrorSuccess) {
                            StringVector vals;
                            pFeature->GetValues(vals);
                            for (uint i =0;i<vals.size();i++) {
                                if (QString::fromStdString(vals[i])=="FixedRate") {
                                    pFeature->SetValue("FixedRate");
                                }
                            }
                        }

                        // get Camera timestamp frequency
                        err=pCamera->GetFeatureByName("GevTimestampTickFrequency",pFeature);
                        if (err==VmbErrorSuccess) {
                            err=pFeature->GetValue(camFreq);
                            if (err==VmbErrorSuccess) {
                                qDebug()<<"Camera freq is "<<(1.0*camFreq);
                            } else {
                                qDebug()<<"Could not extract freq: "<<err;
                            }
                        } else {
                            qDebug()<<"Could not query frequency: "<<err<<" => Will use LUT";
                            if (namestr=="GE1050") {
                                camFreq=79861111;
                            } else if (namestr=="GE1910") {
                                camFreq=79861111;
                            } else if (namestr=="GE2040") {
                                camFreq=79861111;
                            } else {
                                qDebug()<<"Model not yet in LUT => unreliable timestamps";
                                camFreq=79861111;
                            }
                        }


                        // Set acquisition mode to continuous
                        err=pCamera->GetFeatureByName("AcquisitionMode",pFeature);
                        if (err==VmbErrorSuccess) {
                            pFeature->SetValue("Continuous"); // this should be continuous
                        }

                        // set/get maximum height and width of current camera
                        err=pCamera->GetFeatureByName("WidthMax",pFeature);
                        if (err==VmbErrorSuccess) {
                            pFeature->GetValue(maxWidth);
                        }

/*                        err=pCamera->GetFeatureByName("Width",pFeature);
                        if (err==VmbErrorSuccess) {
                            pFeature->SetValue(maxWidth);
                            pFeature->GetValue(width); // this should be continuous
                        }
*/
                        err=pCamera->GetFeatureByName("HeightMax",pFeature);
                        if (err==VmbErrorSuccess) {
                            pFeature->GetValue(maxHeight);
                        }

/*                        err=pCamera->GetFeatureByName("Height",pFeature);
                        if (err==VmbErrorSuccess) {
                            pFeature->SetValue(maxHeight);
                            pFeature->GetValue(height); // this should be continuous
                        }
*/
                        // make sure shutter time is manual
                        err=pCamera->GetFeatureByName("ExposureAuto",pFeature);
                        if (err==VmbErrorSuccess) {
                            pFeature->SetValue("Off"); // this should be manual exposure setting
                        }

                        // now let the user select the pixel format to be used
                        std::vector<std::string> pixF;
                        QStringList items;
                        pixF=listPixelFormats();
                        for (uint i=0;i<pixF.size();i++) {
                            if (pixF[i]=="Mono8") {
                                items<<"MONO8";
                            } else if (pixF[i]=="Mono12") {
                                items<<"MONO12";
                            } else if (pixF[i]=="Mono14") {
                                items<<"MONO14";
                            } else if (pixF[i]=="BayerRG8") {
                                items<<"BAYERRG8";
                            } else if (pixF[i]=="BayerGB8") {
                                items<<"BAYERGB8";
                            } else if (pixF[i]=="BayerRG12") {
                                items<<"BAYERRG12";
                            } else if (pixF[i]=="RGB8Packed") {
                                items<<"RGB8Packed";
                            } else {
                                if (!QString::fromStdString(pixF[i]).contains("12Packed")) {
                                    qDebug()<<"This pixel-mode not yet available in Gigaviewer: "<<QString::fromStdString(pixF[i]);
                                }
                            }

                        }

                        bool ok;
                        QString item = QInputDialog::getItem(NULL, "Pixel format",
                                                            "Selection options:", items, 0, false, &ok);
                        if (ok && !item.isEmpty()) {
                            format=item;
                            setFormat(format);
//                            qDebug()<<"Selected "<<format;
                        }




                        // construct the frame observer to the camera
                        frameWatcher=new VimbaFrameObserver( pCamera, parent );

                    } else {
                        // camera did not open successfully
                        return false;
                    }
                }
            } else {
                qDebug()<<"Zero cameras found";
                return false;
            }



        } else {
            qDebug() << "Could not list cameras. Error code: " << err;
            return false;
        }

    } else {
        qDebug() << "Could not start system. Error code: " << err;
        return false;
    }

    return true;

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
//    qDebug()<<"Format is: "<<format;
    if (indexIncrement<0) qDebug()<<"Cannot stream backwards";

    AVT::VmbAPI::FramePtr pFrame=frameWatcher->GetFrame();
    VmbErrorType err;
    VmbUint32_t width,height;
    VmbPixelFormatType pixFormat;
    err=pFrame->GetPixelFormat(pixFormat);
    err=pFrame->GetHeight(height);
    err=pFrame->GetWidth(width);
//    qDebug()<<"Received frame with size: "<<width<<"x"<<height;
    if ((pixFormat==VmbPixelFormatMono8)||(pixFormat==VmbPixelFormatBayerRG8)||(pixFormat==VmbPixelFormatBayerGB8)) {
        target.image=cv::Mat(height,width,CV_8U);
        err=pFrame->GetImage(target.image.data); // assign the frame image buffer pointer to the target image
        if (err!=VmbErrorSuccess) {
            qDebug()<<"Something went wrong assigning the data";
        }
        if (pixFormat==VmbPixelFormatMono8) target.pixFormat="MONO8";
        if (pixFormat==VmbPixelFormatBayerRG8) target.pixFormat="BAYERRG8";
        if (pixFormat==VmbPixelFormatBayerGB8) target.pixFormat="BAYERGB8";
    } else if ((pixFormat==VmbPixelFormatMono12)||(pixFormat==VmbPixelFormatMono14)||(pixFormat==VmbPixelFormatBayerRG12)) {
        target.image=cv::Mat(height,width,CV_16U);
        err=pFrame->GetImage(target.image.data);
        if (err!=VmbErrorSuccess) {
            qDebug()<<"Something went wrong with the reception of the 12 bit image: "<<err;
        } else {
            //qDebug()<<"12-bit image should be moving up the stack";
        }
        if (pixFormat==VmbPixelFormatMono12) target.pixFormat="MONO12";
        if (pixFormat==VmbPixelFormatMono14) target.pixFormat="MONO14";
        if (pixFormat==VmbPixelFormatBayerRG12) target.pixFormat="BAYERRG12";
    } else if (pixFormat==VmbPixelFormatRgb8) {
        target.image=cv::Mat(height,width,CV_8UC3);
        err=pFrame->GetImage(target.image.data); // assign the frame image buffer pointer to the target image
        if (err!=VmbErrorSuccess) {
            qDebug()<<"Something went wrong assigning the data";
        }
        if (pixFormat==VmbPixelFormatRgb8) target.pixFormat="RGB8";

    } else {
        qDebug()<<"Other pixel formats not yet working";
    }

//    qDebug()<<"target format is: "<<target.pixFormat;

    VmbUint64_t id;
    err=pFrame->GetFrameID(id);
    target.seqNumber=id;

    VmbUint64_t stamp;
    err=pFrame->GetTimestamp(stamp);
    if (initialStamp==0) {
        initialStamp=stamp;
        qint64 currMsec=QDateTime::currentMSecsSinceEpoch();
        timeOffset=currMsec;
//        qDebug()<<"StartTime: "<<currMsec<<" vs "<<timeOffset;
    }
    target.timeStamp=timeOffset+1000.0*(stamp-initialStamp)/(1.0*camFreq);
//        qDebug()<<"Time Stamp in ms: "<<(target.timeStamp-timeOffset);

    pCamera->QueueFrame( pFrame ); // requeue here. Not sure what will happen if buffer too small!

    return true;

}

int VimbaSourceSink::SetInterval(int msec)
{
    FeaturePtr pFeature;
    double acqRate=1000.0/(1.0*msec);
    VmbErrorType err=pCamera->GetFeatureByName("AcquisitionFrameRateAbs",pFeature);
    if (err!=VmbErrorSuccess) {
        err=pCamera->GetFeatureByName("AcquisitionFrameRate",pFeature);
    }
    if (err==VmbErrorSuccess) {
        qDebug()<<"Could find the framerate handle";
        err=pFeature->SetValue(acqRate); // this should be continuous
        if (err==VmbErrorSuccess) {
            frameRate=acqRate;
            qDebug()<<"New frame rate is: "<<frameRate;
            return msec;
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
                    frameRate=acqRate;
                    qDebug()<<"actual framerate now: "<<frameRate;
                    return (int)(1000.0/acqRate);
                }
            }
        }
    }
    qDebug()<<"Could not find the framerate handle";
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
    } else {
        // perhaps an old firewire camera (AVT Firepackage)
        err=pCamera->GetFeatureByName("ExposureTime",pFeature);
        if (err==VmbErrorSuccess) {
            err=pFeature->SetValue(dShut);
            if (err==VmbErrorSuccess) {
                exposure=shutTime;
                qDebug()<<"ExposureTime worked";
                return true;
            }
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
        while (!succ && (counter<10)) {
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

std::vector<std::string> VimbaSourceSink::listPixelFormats() {
    FeaturePtr pFeature;
    VmbErrorType err;
    err = pCamera->GetFeatureByName( "PixelFormat", pFeature );
    if ( VmbErrorSuccess == err ) {
        return listOptions(pFeature);
    } else {
        StringVector vals;
        return vals;
    }
}

std::vector<std::string> VimbaSourceSink::listOptions(FeaturePtr pFeature) {
    StringVector vals;
    StringVector realVals;
    pFeature->GetValues(vals);
    for (uint i=0;i<vals.size();i++) {
        bool ok=false;
        pFeature->IsValueAvailable(vals[i].c_str(),ok);
        if (ok) {
            realVals.push_back(vals[i]);
//            qDebug()<<"Valid value: " << QString::fromStdString(vals[i]);
        }
    }
    return realVals;
}

bool VimbaSourceSink::SetRoiRows(int rows) {
    qDebug()<<"Setting the rows";
    if (rows>maxHeight) {
        rows=maxHeight;
    }
    FeaturePtr pFeature;
    VmbErrorType err;
    err=pCamera->GetFeatureByName("Height",pFeature);
    if (err==VmbErrorSuccess) {
        err=pFeature->SetValue(rows);
        if (err==VmbErrorSuccess) {
            height=rows;
            qDebug()<<"Should be here";
        } else {
            if (err==VmbErrorInvalidAccess) { // probably not possible to set ROI while images are streaming
                err=pCamera->StopContinuousImageAcquisition();
                if (err==VmbErrorSuccess) {
                    err=pCamera->GetFeatureByName("Height",pFeature);
                    if (err==VmbErrorSuccess) {
                        err=pFeature->SetValue(rows);
                        if (err==VmbErrorSuccess) {
//                            err=pCamera->StartContinuousImageAcquisition(bufCount,IFrameObserverPtr(frameWatcher));
                            height=rows;
                            if (err==VmbErrorSuccess) {
                                qDebug()<<"Restart successfull";
                            } else {
                                qDebug()<<"Restart did not work"<<err;
                            }

                        }
                    }
                }
            } else {
                qDebug()<<"Or perhaps here";
            }

        }

    } else {
        qDebug()<<"Setting Rows did not work";
    }
    return true;
}

bool VimbaSourceSink::SetRoiCols(int cols) {
    if (cols==width) {
        return true; //nothing to do
        qDebug()<<"Nothing to do for columns";
    }
    if (cols>maxWidth) {
        cols=maxWidth;
    }
    FeaturePtr pFeature;
    VmbErrorType err;
    err=pCamera->GetFeatureByName("Width",pFeature);
    if (err==VmbErrorSuccess) {
        err=pFeature->SetValue(cols);
        if (err==VmbErrorSuccess) {
            width=cols;
        } else {
            if (err==VmbErrorInvalidAccess) { // probably not possible to set ROI while images are streaming
                err=pCamera->StopContinuousImageAcquisition();
                if (err==VmbErrorSuccess) {
                    err=pCamera->GetFeatureByName("Width",pFeature);
                    if (err==VmbErrorSuccess) {
                        err=pFeature->SetValue(cols);
                        if (err==VmbErrorSuccess) {
//                            err=pCamera->StartContinuousImageAcquisition(bufCount,IFrameObserverPtr(frameWatcher));
                            width=cols;
                            if (err==VmbErrorSuccess) {
                                qDebug()<<"Restart successfull";
                            } else {
                                qDebug()<<"Restart did not work"<<err;
                            }

                        }
                    }
                }
            }

        }
    } else {
        qDebug()<<"Setting Cols did not work";
    }
    return true;
}

