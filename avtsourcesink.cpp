#include "avtsourcesink.h"

bool AvtSourceSink::RecordFrame(ImagePacket&)
{
    return FALSE;
}

bool AvtSourceSink::StartRecording(QString, QString, int, int, int)
{
    return FALSE;
}

bool AvtSourceSink::StopRecording()
{
    return FALSE;
}

bool AvtSourceSink::IsOpened()
{
    return TRUE;
}

bool AvtSourceSink::Init()
{
    tPvErr errCode;

    // initialize the PvAPI
    if((errCode = PvInitialize()) != ePvErrSuccess) {
        qDebug()<<"PvInitialize err:"<< errCode;
        return FALSE;
    }

    //IMPORTANT: Initialize camera structure. See tPvFrame in PvApi.h for more info.
    memset(&GCamera,0,sizeof(tCamera));

    tPvUint32 count,connected;
    tPvCameraInfo list;

    if (!PvCameraCount()) {
        Sleeper::msleep(2000);
        if (!PvCameraCount()) return FALSE;
    }
    bool failed = FALSE;
    count = PvCameraList(&list,1,&connected);
    if(count == 1)
    {
        GCamera.UID = list.UniqueId;
        qDebug()<<"grabbing camera"<<list.SerialString;

        tPvErr errCode;
        unsigned long FrameSize = 0;

        if ((errCode = PvCameraOpen(GCamera.UID,ePvAccessMaster,&(GCamera.Handle))) != ePvErrSuccess)
        {
            if (errCode == ePvErrAccessDenied)
                qDebug()<<"PvCameraOpen returned ePvErrAccessDenied"<<"Camera already open as Master, or camera wasn't properly closed and still waiting to HeartbeatTimeout.";
            else
                qDebug()<<"PvCameraOpen err:"<< errCode;
            return FALSE;
        }

        // Calculate frame buffer size
        if((errCode = PvAttrUint32Get(GCamera.Handle,"TotalBytesPerFrame",&FrameSize)) != ePvErrSuccess)
        {
            qDebug()<<"CameraSetup: Get TotalBytesPerFrame err:"<< errCode;
            return FALSE;
        }

        unsigned long width,height;

        if((errCode = PvAttrUint32Get(GCamera.Handle,"Width",&width)) != ePvErrSuccess)
        {
            qDebug()<<"CameraSetup: Get width err:"<< errCode;
            return FALSE;
        }
        cols=width;

        if((errCode = PvAttrUint32Get(GCamera.Handle,"Height",&height)) != ePvErrSuccess)
        {
            qDebug()<<"CameraSetup: Get height err:"<< errCode;
            return FALSE;
        }
        rows=height;

        unsigned long camFrequency;
        if((errCode = PvAttrUint32Get(GCamera.Handle,"TimeStampFrequency",&camFrequency)) != ePvErrSuccess)
        {
            qDebug()<<"CameraSetup: Get freq err:"<< errCode;
            return FALSE;
        }
        camTimeStep=1.0/((double)camFrequency);
//        qDebug()<<"Camera frequency"<<camTimeStep;



        char pixelFormat[256];
        if((errCode = PvAttrEnumGet(GCamera.Handle,"PixelFormat",pixelFormat,256,NULL)) != ePvErrSuccess)
        {
            qDebug()<<"CameraSetup: Get PixelFormat err:"<<errCode;
            return FALSE;
        }

        qDebug()<<"Image format:"<<pixelFormat<<","<<cols<<","<<rows;

        const int bufferSize[] = {FRAMESCOUNT, rows,cols};

        if (strcmp(pixelFormat,"Mono8")==0) {
//            qDebug()<<"8bit images";
            buffer=cv::Mat(3,bufferSize,CV_8U);
            matFrames=QVector<cv::Mat>(FRAMESCOUNT);

            for (int i=0;i<FRAMESCOUNT;i++) {
                    matFrames[i]=cv::Mat(rows,cols,CV_8U,&buffer.data[i*buffer.step[0]]);
            }

        } else if (strcmp(pixelFormat,"Mono16")==0) {
            qDebug()<<"16bit images not yet implemented";
            return FALSE;
        } else {
            qDebug()<<"Unknown format:"<<pixelFormat;
            return FALSE;
        }

        // assign the frame buffers to the created cv::Mat's
        for(int i=0;i<FRAMESCOUNT && !failed;i++)
        {
            GCamera.Frames[i].ImageBuffer = matFrames[i].data;
            if(GCamera.Frames[i].ImageBuffer)
            {
                GCamera.Frames[i].ImageBufferSize = FrameSize;
            }
            else
            {
                qDebug()<<"CameraSetup: Failed to allocate buffers.";
                failed = TRUE;
            }
        }

        // NOTE: This call sets camera PacketSize to largest sized test packet, up to 8228, that doesn't fail
        // on network card. Some MS VISTA network card drivers become unresponsive if test packet fails.
        // Use PvUint32Set(handle, "PacketSize", MaxAllowablePacketSize) instead. See network card properties
        // for max allowable PacketSize/MTU/JumboFrameSize.
        if((errCode = PvCaptureAdjustPacketSize(GCamera.Handle,8228)) != ePvErrSuccess)
        {
            qDebug()<<"CameraStart: PvCaptureAdjustPacketSize err:"<<errCode;
            return FALSE;
        }
        return !failed;
    } else {
        qDebug()<<"Multiple cameras not yet supported";
        return FALSE;
    }
}

bool AvtSourceSink::StartAcquisition(QString dev)
{
    if (dev!="AVT") qDebug()<<"Different devices not yet implemented";

    tPvErr errCode;
    bool failed = FALSE;
    Index=0;
    Last=0;
    // start driver capture stream
    if((errCode = PvCaptureStart(GCamera.Handle)) != ePvErrSuccess)
    {
        qDebug()<<"CameraStart: PvCaptureStart err:"<< errCode;
        return FALSE;
    }

    // queue frames. No FrameDoneCB callback function.
    for(int i=0;i<FRAMESCOUNT && !failed;i++)
    {
        if((errCode = PvCaptureQueueFrame(GCamera.Handle,&(GCamera.Frames[i]),NULL)) != ePvErrSuccess)
        {
            qDebug()<<"CameraStart: PvCaptureQueueFrame err:"<<errCode;
            failed = TRUE;
        }
    }
    if (failed)
        return FALSE;

    // set the camera in freerun trigger, continuous mode, and start camera receiving triggers
    if((PvAttrEnumSet(GCamera.Handle,"FrameStartTriggerMode","FixedRate") != ePvErrSuccess) ||
        (PvAttrEnumSet(GCamera.Handle,"AcquisitionMode","Continuous") != ePvErrSuccess) ||
        (PvCommandRun(GCamera.Handle,"AcquisitionStart") != ePvErrSuccess))
    {
        qDebug()<<"CameraStart: failed to set camera attributes";
        return FALSE;
    }

    /* if you want to see a list of all attributes
    tPvAttrListPtr listPtr;
    unsigned long listLength;
    if (PvAttrList(GCamera.Handle, &listPtr, &listLength) == ePvErrSuccess)
    {
        for (int i = 0; abs(i) < listLength; i++)
        {
            const char* attributeName = listPtr[i];
            printf("Attribute %s\n", attributeName);
        }
    }
    */

    camTimeOffset=0;

    //this is to make it correspond to the default value in the gui
    SetInterval(100);

    return TRUE;
}

bool AvtSourceSink::StopAcquisition()
{
    tPvErr errCode;

    //stop camera receiving triggers
    if ((errCode = PvCommandRun(GCamera.Handle,"AcquisitionStop")) != ePvErrSuccess)
        qDebug()<<"AcquisitionStop command err:"<< errCode;
//    else
//        qDebug()<<"AcquisitionStop success.";

    //PvCaptureQueueClear aborts any actively written frame with Frame.Status = ePvErrDataMissing
    //Further queued frames returned with Frame.Status = ePvErrCancelled

    //Add delay between AcquisitionStop and PvCaptureQueueClear
    //to give actively written frame time to complete
    Sleeper::msleep(200);

//    qDebug()<<"Calling PvCaptureQueueClear...";
    if ((errCode = PvCaptureQueueClear(GCamera.Handle)) != ePvErrSuccess)
        qDebug()<<"PvCaptureQueueClear err:"<< errCode;
//    else
//        qDebug()<<"...Queue cleared.";

    //stop driver stream
    if ((errCode = PvCaptureEnd(GCamera.Handle)) != ePvErrSuccess)
        qDebug()<<"PvCaptureEnd err:"<< errCode;
//    else
//        qDebug()<<"Driver stream stopped.";

    return TRUE;
}

bool AvtSourceSink::ReleaseCamera()
{
    tPvErr errCode;

    if((errCode = PvCameraClose(GCamera.Handle)) != ePvErrSuccess)
    {
        qDebug()<<"CameraUnSetup: PvCameraClose err:"<<errCode;
    }
    // delete image buffers => not necessary anymore

    GCamera.Handle = NULL;

    PvUnInitialize();
    return TRUE;
}

bool AvtSourceSink::GrabFrame(ImagePacket &target, int indexIncrement)
{
    if (indexIncrement<0) qDebug()<<"Cannot stream backwards";

    tPvErr errCode;
    bool failed=FALSE;


    while((errCode = PvCaptureWaitForFrameDone(GCamera.Handle,&GCamera.Frames[Index],2000)) == ePvErrTimeout)
                qDebug()<<"Waiting for frame to return to host...";

    if(errCode != ePvErrSuccess) {
        //likely camera unplugged
        qDebug()<<"PvCaptureWaitForFrameDone err:"<< errCode;
        failed = TRUE;
    } else {
        // if frame hasn't been cancelled, requeue frame
        if(GCamera.Frames[Index].Status != ePvErrCancelled) {
            //Check for gaps in FrameCount due to image returning from camera with no frame queued.
            //This should never happen, as we use a multiple frame circular buffer.
            if(Last + 1 != GCamera.Frames[Index].FrameCount) {
                //Note missing frame
                GCamera.Discarded++;
            }

            Last = GCamera.Frames[Index].FrameCount;

            if (GCamera.Frames[Index].Status != ePvErrSuccess) {
                if (GCamera.Frames[Index].Status==ePvErrDataMissing) {
                    qDebug()<<"Data missing in frame"<<GCamera.Frames[Index].FrameCount;
                } else {
                    qDebug()<<"Frame - Error:"<< GCamera.Frames[Index].FrameCount<<"-"<< GCamera.Frames[Index].Status;
                }
            } else {
                //shallow copy of image to target.image (sam)
                target.image=matFrames[Index];
                target.seqNumber=GCamera.Frames[Index].FrameCount;
                unsigned long ts;
                ts=(((unsigned long)GCamera.Frames[Index].TimestampHi)<<32) + GCamera.Frames[Index].TimestampLo;
                if (camTimeOffset==0.0) {
                    qint64 currMsec=QDateTime::currentMSecsSinceEpoch();
                    camTimeOffset=currMsec-((double)ts)*camTimeStep;
                }
                target.timeStamp=((double)ts)*camTimeStep+camTimeOffset;
                //qDebug()<<ts<<target.timeStamp;

            }

            //Requeue [Index] frame of FRAMESCOUNT num frames
            if ((errCode = PvCaptureQueueFrame(GCamera.Handle,&GCamera.Frames[Index],NULL)) != ePvErrSuccess) {
                qDebug()<<"PvCaptureQueueFrame err:"<< errCode;
                failed = TRUE;
            }

            //Increment [Index]
            Index++;
            if(Index==FRAMESCOUNT)
                Index = 0;
        } else {
            //Cancelled
            failed = TRUE;
        }
    }
    return !failed;

}

bool AvtSourceSink::SetInterval(int msec)
{
    double fps = 1000.0/((double)msec);
    tPvFloat32 fpsFloat=1.0;
    if (fps>1) {
        fpsFloat=(int)fps;
        qDebug()<<"Rounded framerate will be"<<fpsFloat;
    } else {
        fpsFloat=fps;
    }
    tPvErr errCode;

    if ((errCode=PvAttrFloat32Set(GCamera.Handle,"FrameRate",fpsFloat))!=ePvErrSuccess) {
        qDebug()<<"Setting the frame rate to"<<fpsFloat<<"did not work";
        return FALSE;
    }
    return TRUE;
}

bool AvtSourceSink::SetShutter(int shutTime)
{
    tPvUint32 val=abs(shutTime);
    char buf[32];
    unsigned long enum_size;
    tPvErr errCode;
    tPvUint32 min, max;

    if ((errCode=PvAttrEnumGet(GCamera.Handle,"ExposureMode",buf,32,&enum_size))!=ePvErrSuccess) {
        qDebug()<<"Could not get exposure mode";
    }
    if (strncmp(buf,"Manual",enum_size)!=0) {
        if ((errCode=PvAttrEnumSet(GCamera.Handle,"ExposureMode","Manual"))!=ePvErrSuccess) {
            qDebug()<<"Failed to set shutter to Manual mode";
        }

    }
    if (strncmp(buf,"Manual",enum_size)==0) {
        if ((errCode=PvAttrRangeUint32(GCamera.Handle,"ExposureValue",&min,&max))!=ePvErrSuccess) {
            qDebug()<<"Could not get range, using default";
            min=50;
            max=60000000;
        }

        if (val<min) {
            qDebug()<<"Minimum allowed shutter time is"<<min;
        } else if (val>max) {
            qDebug()<<"Maximum allowed shutter time is"<<max;
        } else {
            if ((errCode=PvAttrUint32Set(GCamera.Handle,"ExposureValue",val))!=ePvErrSuccess) {
                qDebug()<<"Could not set exposure value to"<<shutTime<<"usec";
            } else {
                return TRUE;
            }
        }
    }


    return FALSE;
}

int AvtSourceSink::SetAutoShutter(bool fitRange)
{
    tPvErr errCode;
    tPvUint32 oldval=0;
    if ((errCode=PvAttrUint32Get(GCamera.Handle,"ExposureValue",&oldval))!=ePvErrSuccess) {
        qDebug()<<"Could not get initial exposure value";
    }

    if (fitRange) {
        if ((errCode=PvAttrEnumSet(GCamera.Handle,"ExposureAutoAlg","FitRange"))!=ePvErrSuccess) {
            qDebug()<<"Could not set exposure auto algorithm";
            return 0;
        }
    } else {
        if ((errCode=PvAttrEnumSet(GCamera.Handle,"ExposureAutoAlg","Mean"))!=ePvErrSuccess) {
            qDebug()<<"Could not set exposure auto algorithm";
            return 0;
        }
    }

    if ((errCode=PvAttrEnumSet(GCamera.Handle,"ExposureMode","AutoOnce"))!=ePvErrSuccess) {
        qDebug()<<"Failed to set shutter to AutoOnce mode";
        return 0;
    }

    //now wait till exposure mode changes to propagate this info back to the gui

    tPvUint32 newval=0;
    if ((errCode=PvAttrUint32Get(GCamera.Handle,"ExposureValue",&newval))!=ePvErrSuccess) {
        qDebug()<<"Could not get exposure value";
    }

    if (newval!=oldval) {
        return newval;
    } else {
        //should sleep a bit before reasking
        if ((errCode=PvAttrUint32Get(GCamera.Handle,"ExposureValue",&newval))!=ePvErrSuccess) {
            qDebug()<<"Could not get exposure value";
        }
    }

    return newval;

}
