#include "idssourcesink.h"

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
        qDebug()<<"No camera found";
        return false;
    } else if (m_pCamList->dwCount>1) {
        qDebug()<<"More than 1 camera: "<<m_pCamList->dwCount;
    }

    // will use camera 0
    memcpy (&m_CameraInfo, &m_pCamList->uci[0], sizeof(UEYE_CAMERA_INFO));
    hCam = (HIDS) (m_CameraInfo.dwDeviceID | IS_USE_DEVICE_ID);


    if(is_InitCamera (&hCam, NULL)!= IS_SUCCESS){
        qDebug()<<"init not successful";
           return false;
    }

//    double minFPS, maxFPS, FPSinterval;
//    is_GetFrameTimeRange (hCam, &minFPS, &maxFPS, &FPSinterval);
         //cout<< fixed << setprecision(4) << minFPS << " MINFPS " << maxFPS << " MAXFPS "<< FPSinterval << " FPSinterval " << endl;
         //myfile<< fixed << setprecision(4) << minFPS << " MINFPS " << maxFPS << " MAXFPS "<< FPSinterval << " FPSinterval " << endl;

    is_SetGainBoost (hCam, IS_SET_GAINBOOST_OFF);
    is_SetWhiteBalance (hCam, IS_SET_WB_DISABLE);
//    is_SetBrightness (hCam,0);
//    is_SetContrast (hCam,0);
//    is_SetGamma (hCam, 100);// Value multiplied by 100 (for the camera it goes from 0.01 to 10
    is_SetHWGainFactor (hCam, IS_SET_MASTER_GAIN_FACTOR, 100);
    uint pixelC=304;
    is_PixelClock(hCam, IS_PIXELCLOCK_CMD_SET, (void*)&pixelC, sizeof(pixelC));

    flagIDS= is_SetSubSampling (hCam, IS_SUBSAMPLING_2X_VERTICAL | IS_SUBSAMPLING_2X_HORIZONTAL); //Both are needed

    //Configuration section: very important to match the img_bpp=8 with the chacracteristics of the CV::MAT image to use
    //weird results like cropping or black lines can be obtained if not changed accordingly
    int img_width=2048, img_height=2048, img_bpp=8, factorSMP=2; //Variable to state the Linehopping
//    int img_step, img_data_size;

    imgMem = NULL;
    is_AllocImageMem(hCam, img_width/factorSMP, img_height/factorSMP, img_bpp, &imgMem, &memId);
    is_SetImageMem (hCam, imgMem, memId);
//    is_SetImageSize (hCam, img_width/factorSMP, img_height/factorSMP);

    is_SetColorMode (hCam, IS_CM_MONO8);

    is_SetDisplayMode (hCam, IS_SET_DM_DIB); // Direct buffer mode writes to RAM which is the only option on Linux

    //OpenCV variables: REMEMBER THE SUBSAMPLING
    buffer=cv::Mat::zeros(img_width/factorSMP,img_height/factorSMP, CV_8UC1);

    return true;
}

bool IdsSourceSink::StartAcquisition(QString dev)
{
    if (dev!="IDS") qDebug()<<"Different devices not yet implemented";

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
    if (indexIncrement<0) qDebug()<<"Cannot stream backwards";

#ifdef Q_OS_WIN32
    if (WaitForSingleObject(hEvent, camTimeStep*5) == WAIT_OBJECT_0) {
#else
    if (is_WaitEvent (hCam, IS_SET_EVENT_FRAME, camTimeStep*5) == IS_SUCCESS) {
#endif
        void *pMemVoid; //pointer to where the image is stored  **********************
        is_GetImageMem (hCam, &pMemVoid);
        buffer.data =  (unsigned char*)pMemVoid;
        is_GetImageInfo( hCam, memId, &ImageInfo, sizeof(ImageInfo)); //Get info ASAP in case of an interruption due to a click
        target.image=buffer.clone();

        if (camTimeOffset==0) {
            camTimeOffset=3600000*ImageInfo.TimestampSystem.wHour+60000*ImageInfo.TimestampSystem.wMinute+1000.0*ImageInfo.TimestampSystem.wSecond+ImageInfo.TimestampSystem.wMilliseconds;
            target.timeStamp=0;
        } else {
            double tis;
            tis=3600000*ImageInfo.TimestampSystem.wHour+60000*ImageInfo.TimestampSystem.wMinute+1000.0*ImageInfo.TimestampSystem.wSecond+ImageInfo.TimestampSystem.wMilliseconds-camTimeOffset;
            target.timeStamp=tis;
        }
        target.seqNumber=ImageInfo.u64FrameNumber;

        target.pixFormat="MONO8";

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
    idsSleeper::msleep(10*camTimeStep); // NEEDS some sleep to stop all internal events
    return true;
}

bool IdsSourceSink::ReleaseCamera() {
    is_ExitCamera(hCam);
    hCam=(HIDS)0;
    return true;
}

int IdsSourceSink::SetInterval(int msec) {
    double fps = 1000.0/((double)msec);
    double newfps;
    is_SetFrameRate(hCam,fps,&newfps);
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

    if (texp<dblRange[0]) texp=dblRange[0];

    is_Exposure(hCam,IS_EXPOSURE_CMD_SET_EXPOSURE,(void*)&texp,sizeof(texp));

    //is_SetExposureTime (hCam, IS_GET_EXPOSURE_TIME, &texp);

    return true;
}

int IdsSourceSink::SetAutoShutter(bool fitRange)
{
    if ((fitRange)||(!fitRange)) {
        qDebug()<<"The auto shutter mode parameter has no effect for IDS";
    }
    double dEnable=1.0;
    is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_SHUTTER, &dEnable, NULL);
    return 0;
}
