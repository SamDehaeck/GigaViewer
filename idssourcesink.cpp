#include "idssourcesink.h"
#include <QInputDialog>

bool IdsSourceSink::IsOpened()
{
    return true;
}

bool IdsSourceSink::Init()
{

    PUEYE_CAMERA_LIST m_pCamList;
    UEYE_CAMERA_INFO m_CameraInfo;
    // init the internal camera info structure
    ZeroMemory (&m_CameraInfo, sizeof(UEYE_CAMERA_INFO));

    // get the cameralist from SDK
    m_pCamList = new UEYE_CAMERA_LIST;
    m_pCamList->dwCount = 0;

    if (is_GetCameraList (m_pCamList) == IS_SUCCESS) {
            DWORD dw = m_pCamList->dwCount;
            delete m_pCamList;

            // Reallocate the required camera list size
            m_pCamList = (PUEYE_CAMERA_LIST)new char[sizeof(DWORD) + dw * sizeof(UEYE_CAMERA_INFO)];
            m_pCamList->dwCount = dw;

            // Get CameraList and store it ...
            if (is_GetCameraList (m_pCamList) != IS_SUCCESS) return false;
    } else return false;

    if (m_pCamList->dwCount==0) {
        qInfo()<<"No camera found";
        return false;
    } else if (m_pCamList->dwCount>1) {
        qInfo()<<"More than 1 camera: "<<m_pCamList->dwCount;
    }

    // will use camera 0
    memcpy (&m_CameraInfo, &m_pCamList->uci[0], sizeof(UEYE_CAMERA_INFO));
    hCam = (HIDS) (m_CameraInfo.dwDeviceID | IS_USE_DEVICE_ID);


    if(is_InitCamera (&hCam, NULL)!= IS_SUCCESS){
        qInfo()<<"init not successful";
           return false;
    }

    bool succ=SetPixelClock();
    if (!succ) {
        qWarning()<<"Something went wrong with the Pixel Clock";
    }

    //Configuration section: very important to match the img_bpp=8 with the chacracteristics of the CV::MAT image to use
    //weird results like cropping or black lines can be obtained if not changed accordingly

    int img_width=0, img_height=0; //Variable to state the Linehopping

    SENSORINFO sInfo;
    is_GetSensorInfo (hCam, &sInfo);
    img_width = sInfo.nMaxWidth;
    img_height = sInfo.nMaxHeight;
    qInfo()<<"Width is "<<img_width<<" and height is "<<img_height;
    maxWidth=img_width;
    cols=img_width;
    maxHeight=img_height;
    rows=img_height;

    //    is_SetGainBoost (hCam, IS_SET_GAINBOOST_OFF);
    //    is_SetWhiteBalance (hCam, IS_SET_WB_DISABLE);
    //    is_SetBrightness (hCam,0);
    //    is_SetContrast (hCam,0);
    //    is_SetGamma (hCam, 100);// Value multiplied by 100 (for the camera it goes from 0.01 to 10
    //    is_SetHWGainFactor (hCam, IS_SET_MASTER_GAIN_FACTOR, 100);


    // Set Gain of camera
    bool masterGain=sInfo.bMasterGain;
//    qInfo()<<"Hardwaregain is present:"<<masterGain;
    if (masterGain) {
        int ret;
        ret = is_SetHWGainFactor(hCam, IS_INQUIRE_MASTER_GAIN_FACTOR, 100);
        qInfo()<<"Maximum gain is: "<<ret; // 100 is no gain => 350 is 3.5x or is this 1x and in how many steps?

        int gainSetpoint=100;

        is_SetHWGainFactor(hCam, IS_SET_MASTER_GAIN_FACTOR, gainSetpoint);
        ret = is_SetHWGainFactor(hCam, IS_GET_MASTER_GAIN_FACTOR, 100);
        qInfo()<<"Now it is: "<<ret;
    }



//    flagIDS= is_SetSubSampling (hCam, IS_SUBSAMPLING_2X_VERTICAL | IS_SUBSAMPLING_2X_HORIZONTAL); //Both are needed
    int factorSMP=1; // change this to value of subsampling

//    int img_step, img_data_size;

    bool ret=SetColourMode(true); // check which colour modes are possible and ask for which one, true for high quality debayering
    if (ret==false) {
        qInfo()<<"Setting of colour mode not successful";
    }

    int img_bpp=8;

    //OpenCV variables: REMEMBER THE SUBSAMPLING
    if (format=="RGB8" || format=="BGR8") {
        buffer=cv::Mat::zeros(img_height/factorSMP,img_width/factorSMP, CV_8UC3);
        img_bpp=24;
    } else {
        buffer=cv::Mat::zeros(img_height/factorSMP,img_width/factorSMP, CV_8UC1);
    }

    imgMem = NULL;
    is_AllocImageMem(hCam, img_width/factorSMP, img_height/factorSMP, img_bpp, &imgMem, &memId);
    is_SetImageMem (hCam, imgMem, memId);
//    is_SetImageSize (hCam, img_width/factorSMP, img_height/factorSMP);

    is_SetDisplayMode (hCam, IS_SET_DM_DIB); // Direct buffer mode writes to RAM which is the only option on Linux

    return true;
}

bool IdsSourceSink::StartAcquisition(QString dev)
{
    if (dev!="IDS") qInfo()<<"Different devices not yet implemented";

    is_SetExternalTrigger (hCam, IS_SET_TRIGGER_OFF); //This makes that calling freeze image creates a new image

#ifdef Q_OS_WIN32
    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    is_InitEvent(hCam, hEvent, IS_SET_EVENT_FRAME);
#endif

    is_EnableEvent(hCam, IS_SET_EVENT_FRAME);
    is_CaptureVideo(hCam, IS_WAIT);


    //this is to make it correspond to the default value in the gui
    SetInterval(100);
    SetShutter(100);

    return true;
}

bool IdsSourceSink::GrabFrame(ImagePacket &target, int indexIncrement)
{
    if (indexIncrement<0) qInfo()<<"Cannot stream backwards";

#ifdef Q_OS_WIN32
    if (WaitForSingleObject(hEvent, camTimeStep*5) == WAIT_OBJECT_0) {
#else
    if (is_WaitEvent (hCam, IS_SET_EVENT_FRAME, camTimeStep*5) == IS_SUCCESS) {
#endif
        void *pMemVoid; //pointer to where the image is stored  **********************
        is_GetImageMem (hCam, &pMemVoid);
        buffer.data =  (unsigned char*)pMemVoid;
        is_GetImageInfo( hCam, memId, &ImageInfo, sizeof(ImageInfo)); //Get info ASAP in case of an interruption due to a click
        target.image=buffer(cv::Rect(0,0,cols,rows)).clone(); // only copy cropped version from buffer to output.

        if (camTimeOffset==0) {
            camTimeOffset=3600000*ImageInfo.TimestampSystem.wHour+60000*ImageInfo.TimestampSystem.wMinute+1000.0*ImageInfo.TimestampSystem.wSecond+ImageInfo.TimestampSystem.wMilliseconds;
            target.timeStamp=0;
        } else {
            double tis;
            tis=3600000*ImageInfo.TimestampSystem.wHour+60000*ImageInfo.TimestampSystem.wMinute+1000.0*ImageInfo.TimestampSystem.wSecond+ImageInfo.TimestampSystem.wMilliseconds-camTimeOffset;
            target.timeStamp=tis;
        }
        target.seqNumber=ImageInfo.u64FrameNumber;

        target.pixFormat=format;

    }

    return true;

}


bool IdsSourceSink::StopAcquisition() {
    is_StopLiveVideo(hCam, IS_WAIT);
    is_DisableEvent (hCam, IS_SET_EVENT_FRAME); //added by Sam
#ifdef Q_OS_WIN32
    is_ExitEvent(hCam, IS_SET_EVENT_FRAME);
#endif
    is_FreeImageMem (hCam, imgMem, memId);
    Sleeper::msleep(10*camTimeStep); // NEEDS some sleep to stop all internal events
    return true;
}

bool IdsSourceSink::ReleaseCamera() {
    is_ExitCamera(hCam);
    hCam=(HIDS)0;
    return true;
}

bool IdsSourceSink::SetPixelClock() {
    // first set the extended pixelclock range if available to reach higher maximal fps
    /* Enable the extended pixel clock range */
    bool worked=false;
    //bool extClock;
    UINT nEnable;
    nEnable = EXTENDED_PIXELCLOCK_RANGE_ON;
    INT nRet = is_DeviceFeature(hCam, IS_DEVICE_FEATURE_CMD_SET_EXTENDED_PIXELCLOCK_RANGE_ENABLE, (void*)&nEnable, sizeof(nEnable));
    if (nRet == IS_SUCCESS) {
    /* Extended pixel clock range is enabled */
        qInfo()<<"Extended pixel clock range is enabled";
      //  extClock=true;
    }


    int clockN;
    is_PixelClock(hCam,IS_PIXELCLOCK_CMD_GET_NUMBER,(void*)&clockN,sizeof(clockN));
    //qInfo()<<"Elements in clocklist: "<<clockN;

#ifdef Q_OS_WIN32
    int clockList[100];
#else
    int clockList[clockN];
#endif

    is_PixelClock(hCam,IS_PIXELCLOCK_CMD_GET_LIST,(void*)&clockList,sizeof(clockList));
    QStringList items;
    for (int i=0;i<clockN;i++) {
        //qInfo()<<"Clock option - "<<clockList[i];
        items<<QString::number(clockList[i]);
    }
//    qInfo()<<"Available clock options: "<<items;
    bool ok;
    QString item = QInputDialog::getItem(NULL, "Pixel Clock",
                                        "Selection options:", items, clockN/2, false, &ok);

    // now handle response and set the camera
    if (ok && !item.isEmpty()) {
        int pixelC=item.toInt();
//        qInfo()<<"Selected: "<<pixelC;
        is_PixelClock(hCam, IS_PIXELCLOCK_CMD_SET, (void*)&pixelC, sizeof(pixelC));
        int clockNow;
        is_PixelClock(hCam,IS_PIXELCLOCK_CMD_GET,(void*)&clockNow,sizeof(clockNow));
//        qInfo()<<"Current clock: "<<clockNow;
        if (clockNow==pixelC) {
            worked=true;
        }
    }


    double minFPS, maxFPS, FPSinterval;
    is_GetFrameTimeRange (hCam, &minFPS, &maxFPS, &FPSinterval);
//    qInfo()<<"minFps: "<<QString::number(minFPS)<<" maxFps: "<<QString::number(maxFPS)<<" and interval: "<<QString::number(FPSinterval);
    qInfo()<<"minFps: "<<QString::number(1/maxFPS)<<" maxFps: "<<QString::number(1/minFPS);

    return worked;
}

bool IdsSourceSink::SetColourMode(bool useHighQuality) {
    bool isColCam=false;
    if (is_SetColorCorrection(hCam,IS_GET_SUPPORTED_CCOR_MODE,0)==0) {
        isColCam=false;
    } else {
        isColCam=true;
    }

    if (isColCam) {
        INT CurrentConvertMode, DefaultConvertMode, SupportedConvertModes;
        //UINT ColorFormat;
        //ColorFormat=IS_CM_SENSOR_RAW8;
        QStringList items;

        if (is_GetColorConverter (hCam, IS_CM_SENSOR_RAW8, &CurrentConvertMode, &DefaultConvertMode, &SupportedConvertModes)
                == IS_SUCCESS) {
            //qInfo()<<"Can record in Raw8";
            //qInfo()<<"Probably a colour camera => showing as Bayer8";
            items<<"BAYERRG8";
        }

        if (is_GetColorConverter (hCam, IS_CM_RGB8_PACKED, &CurrentConvertMode, &DefaultConvertMode, &SupportedConvertModes)
                == IS_SUCCESS) {
            //qInfo()<<"Can record in RGB8";
            items<<"RGB8";
        }
        if (is_GetColorConverter (hCam, IS_CM_BGR8_PACKED, &CurrentConvertMode, &DefaultConvertMode, &SupportedConvertModes)
                == IS_SUCCESS) {
            //qInfo()<<"Can record in BGR8";
            items<<"BGR8";
        }

        if (is_GetColorConverter (hCam, IS_CM_MONO8, &CurrentConvertMode, &DefaultConvertMode, &SupportedConvertModes)
                == IS_SUCCESS) {
            //qInfo()<<"Can record in Mono8";
            items<<"MONO8";
        }

        // now ask for which one you would like
        int ret;
        bool ok;
        QString item = QInputDialog::getItem(NULL, "Pixel format",
                                            "Selection options:", items, 0, false, &ok);

        // now handle response and set the camera
        if (ok && !item.isEmpty()) {
            qInfo()<<"Selected: "<<item;
            format=item;

            if (item=="BAYERRG8") {
                ret = is_SetColorMode(hCam, IS_CM_SENSOR_RAW8);
                return (ret == IS_SUCCESS);
            } else if (item=="RGB8") {
                ret = is_SetColorMode(hCam,IS_CM_RGB8_PACKED);
                if (ret==IS_SUCCESS) {
                    if (useHighQuality) {
                        ret=is_SetColorConverter(hCam,IS_CM_RGB8_PACKED,IS_CONV_MODE_SOFTWARE_5X5);
                    } else {
                        ret=is_SetColorConverter(hCam,IS_CM_RGB8_PACKED,IS_CONV_MODE_SOFTWARE_3X3);
                    }
                }
                return (ret == IS_SUCCESS);
            } else if (item=="BGR8") {
                ret = is_SetColorMode(hCam,IS_CM_BGR8_PACKED);
                if (ret==IS_SUCCESS) {
                    if (useHighQuality) {
                        ret=is_SetColorConverter(hCam,IS_CM_BGR8_PACKED,IS_CONV_MODE_SOFTWARE_5X5);
                    } else {
                        ret=is_SetColorConverter(hCam,IS_CM_BGR8_PACKED,IS_CONV_MODE_SOFTWARE_3X3);
                    }
                }
                return (ret == IS_SUCCESS);
            } else if (item=="RAW8") {
                format="MONO8"; // for monochrome cameras raw8=mono8
                ret = is_SetColorMode(hCam, IS_CM_SENSOR_RAW8);
                return (ret == IS_SUCCESS);
            } else if (item=="MONO8") {
                ret = is_SetColorMode(hCam,IS_CM_MONO8);
                if (ret==IS_SUCCESS) {
                    if (useHighQuality) {
                        ret=is_SetColorConverter(hCam,IS_CM_MONO8,IS_CONV_MODE_SOFTWARE_5X5);
                    } else {
                        ret=is_SetColorConverter(hCam,IS_CM_MONO8,IS_CONV_MODE_SOFTWARE_3X3);
                    }
                }
                return (ret == IS_SUCCESS);
            }
        }

        // now implement the colour correction due to the IR filter
        ret=is_SetColorCorrection (hCam, IS_CCOR_ENABLE_HQ_ENHANCED, 0);
        if (ret!=IS_SUCCESS) {
            qWarning()<<"The colour correction failed";
        }

        qInfo()<<"Accepted colour modes: "<<items;
    } else {
        //it's a monochrome camera => return mono
        int ret;
        format="MONO8";
        ret = is_SetColorMode(hCam, IS_CM_SENSOR_RAW8);
        return (ret == IS_SUCCESS);
    }
    return false;
}

int IdsSourceSink::SetInterval(int msec) {
    double fps = 1000.0/((double)msec);

    double min=0.0,max=0.0,interv=0.0;
    is_GetFrameTimeRange (hCam, &min, &max, &interv);
    double minFps, maxFps;//, minstep;
    maxFps = 1 / min;
    minFps = 1 / max;
//    minstep = (1 / min) - (1 / (min + interv));
//    qInfo()<<"Min fps: "<<minFps<<" max fps: "<<maxFps<<" and interval "<<minstep;
    if (fps<minFps) {
        qInfo()<<"Wanted to go below minimum fps "<<minFps;  // should reduce clock settings to try and reach it
        fps=minFps;
    } else if (fps>maxFps) {
        qInfo()<<"Wanted to go above maximum fps "<<maxFps;  // should increase clock to try and reach it
        fps=maxFps;
    }

    double newfps=0.0;
    is_SetFrameRate(hCam,fps,&newfps);
//    qInfo()<<"Tried to set to: "<<fps<<" and got to "<<newfps;

    camTimeStep=1000.0/newfps;
    return camTimeStep;
}

bool IdsSourceSink::SetShutter(int shutTime)
{
    double dEnable=0.0;
    is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_SHUTTER, &dEnable, NULL); // switch off any autoexposure


    double texp=shutTime/1000.0; // IDS works in msecs

    // get values
    double dblRange[3]; // min, max and increment
    is_Exposure(hCam, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE, (void*)&dblRange, sizeof(dblRange));

    // max exposure time is related to fps and changes each time fps is changed (=> = camtimestep)

//    qInfo()<<"Exposure range is: "<<dblRange[0]<<" to "<<dblRange[1];
//    qInfo()<<"And camtimestep is : "<<camTimeStep;

    if (texp<dblRange[0]) {
        qInfo()<<"Went too low in shutter time: "<<texp<<" vs "<<dblRange[0]; // should increase clock speed to be able to go faster!
        texp=dblRange[0];
    } else if (texp>dblRange[1]) {
        qInfo()<<"Went too high in shutter time: "<<texp<<" vs "<<dblRange[1]; // should decrease clock speed to be able to go slower
        texp=dblRange[1];
    }


    is_Exposure(hCam,IS_EXPOSURE_CMD_SET_EXPOSURE,(void*)&texp,sizeof(texp));

    return true;
}

int IdsSourceSink::SetAutoShutter(bool fitRange)
{
    if (fitRange) {
        qInfo()<<"Will perform white balance instead";
        double dblAutoWb = 1.0;
        is_SetAutoParameter (hCam, IS_SET_AUTO_WB_ONCE, &dblAutoWb, NULL);
        is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &dblAutoWb, NULL);
    }
    double dEnable=1.0;
    is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_SHUTTER, &dEnable, NULL);
    return 0;
}

bool IdsSourceSink::SetRoiRows(int newrows) {
    if (newrows>maxHeight) {
        newrows=maxHeight;
    }

    UINT nAbsPos[2];
    nAbsPos[0]=0;
    nAbsPos[1]=0;

    // set absolute pos
    is_AOI(hCam, IS_AOI_IMAGE_SET_POS, (void*)&nAbsPos , sizeof(nAbsPos));

    IS_SIZE_2D imageSize;
    imageSize.s32Width = cols;
    imageSize.s32Height = newrows;
    INT nRet = is_AOI (hCam,IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize,sizeof(imageSize));
    if (nRet == IS_SUCCESS) {
        rows=newrows;
    } else {
        qInfo()<<"Cropping rows did not work";
    }

    return true;
}

bool IdsSourceSink::SetRoiCols(int newcols) {
    if (newcols>maxWidth) {
        newcols=maxWidth;
    }

    UINT nAbsPos[2];
    nAbsPos[0]=0;
    nAbsPos[1]=0;
    // set absolute pos
    is_AOI(hCam, IS_AOI_IMAGE_SET_POS, (void*)&nAbsPos , sizeof(nAbsPos));

    IS_SIZE_2D imageSize;
    imageSize.s32Width = newcols;
    imageSize.s32Height = rows;
    INT nRet = is_AOI (hCam,IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize,sizeof(imageSize));
    if (nRet == IS_SUCCESS) {
        cols=newcols;
    }  else {
        qInfo()<<"Cropping cols did not work";
    }

    return true;
}


