#include "idssourcesink.h"

bool IdsSourceSink::IsOpened()
{
    return true;
}

bool IdsSourceSink::Init()
{
    qDebug()<<"Got inside IDS!!";

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
        return false;
    } else if (m_pCamList->dwCount>1) {
        qDebug()<<"More than 1 camera: "<<m_pCamList->dwCount;
    }

    // copy current camera info
    memcpy (&m_CameraInfo, &m_pCamList->uci[0], sizeof(UEYE_CAMERA_INFO));


    if(is_InitCamera (&hCam, NULL)!= IS_SUCCESS){
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
//    is_SetPixelClock (hCam, 300);
    flagIDS= is_SetSubSampling (hCam, IS_SUBSAMPLING_2X_VERTICAL | IS_SUBSAMPLING_2X_HORIZONTAL); //Both are needed

    //Configuration section: very important to match the img_bpp=8 with the chacracteristics of the CV::MAT image to use
    //weird results like cropping or black lines can be obtained if not changed accordingly
    int img_width=2048, img_height=2048, img_bpp=8, img_step, img_data_size, factorSMP=2; //Variable to state the Linehopping

    imgMem = NULL;
    is_AllocImageMem(hCam, img_width/factorSMP, img_height/factorSMP, img_bpp, &imgMem, &memId);
    is_SetImageMem (hCam, imgMem, memId);
    is_SetDisplayMode (hCam, IS_SET_DM_DIB);
//    is_SetColorMode (hCam, IS_SET_CM_Y8);
//    is_SetImageSize (hCam, img_width/factorSMP, img_height/factorSMP);

    //OpenCV variables: REMEMBER THE SUBSAMPLING
    buffer=cv::Mat::zeros(img_width/factorSMP,img_height/factorSMP, CV_8UC1);




    return true;
}

bool IdsSourceSink::StartAcquisition(QString dev)
{
    if (dev!="IDS") qDebug()<<"Different devices not yet implemented";

//    is_SetExternalTrigger (hCam, IS_SET_TRIG_OFF); //This makes that calling freeze image creates a new image
/*
    HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    is_InitEvent(hCam, hEvent, IS_SET_EVENT_FRAME);
    is_EnableEvent(hCam, IS_SET_EVENT_FRAME);
    is_CaptureVideo(hCam, IS_WAIT);
*/

    //this is to make it correspond to the default value in the gui
    SetInterval(100);
    SetShutter(100);

    return true;
}

bool IdsSourceSink::GrabFrame(ImagePacket &target, int indexIncrement)
{
    if (indexIncrement<0) qDebug()<<"Cannot stream backwards";
/*
    DWORD dwRet = WaitForSingleObject(hEvent, 1000);
    if (dwRet == WAIT_TIMEOUT) {
                          // wait timed out
    } else if (dwRet == WAIT_OBJECT_0) {
                          // event signalled
        void *pMemVoid; //pointer to where the image is stored  **********************
        is_GetImageMem (hCam, &pMemVoid);
        buffer.data =  (unsigned char*)pMemVoid;
        is_GetImageInfo( hCam, memId, &ImageInfo, sizeof(ImageInfo)); //Get info ASAP in case of an interruption due to a click
    }
*/
/*
                //shallow copy of image to target.image (sam)
                target.image=matFrames[Index];
                target.seqNumber=GCamera.Frames[Index].FrameCount;
                unsigned long ts;
                ts=(((unsigned long)GCamera.Frames[Index].TimestampHi)<<32) + GCamera.Frames[Index].TimestampLo;
                if (camTimeOffset==0.0) {
                    qint64 currMsec=QDateTime::currentMSecsSinceEpoch();
                    camTimeOffset=currMsec-((double)ts)*camTimeStep*1000.0;
                }
                target.timeStamp=((double)ts)*camTimeStep*1000.0+camTimeOffset;
*/
    return true;

}


bool IdsSourceSink::StopAcquisition()
{
    is_StopLiveVideo(hCam, IS_WAIT);
    is_FreeImageMem (hCam, imgMem, memId);
    return true;
}

bool IdsSourceSink::ReleaseCamera()
{
    is_ExitCamera(hCam);
    return true;
}

int IdsSourceSink::SetInterval(int msec)
{
    //is_SetFrameRate(hCam,FPS,&NEWFPS);
    return msec;
}

bool IdsSourceSink::SetShutter(int shutTime)
{
    //is_SetExposureTime (hCam, shutTime*1.0, &texp);

    return false;
}

int IdsSourceSink::SetAutoShutter(bool fitRange)
{
    return 0;
}
