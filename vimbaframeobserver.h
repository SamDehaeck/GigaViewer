#ifndef VIMBAFRAMEOBSERVER
#define VIMBAFRAMEOBSERVER

#include <queue>

#include <QObject>
#include <QMutex>
#include <QDebug>
#include "cambackend.h"

#include <VimbaCPP/Include/VimbaCPP.h>

class VimbaFrameObserver : public QObject, virtual public AVT::VmbAPI::IFrameObserver
{
    Q_OBJECT

  public:
    // We pass the camera that will deliver the frames to the constructor
    VimbaFrameObserver( AVT::VmbAPI::CameraPtr pCamera, CamBackend* consumer);
    
    // This is our callback routine that will be executed on every received frame
    virtual void FrameReceived( const AVT::VmbAPI::FramePtr pFrame );

    // After the camBackend has been notified about a new frame it can pick it up
    AVT::VmbAPI::FramePtr GetFrame();

    // Clears the double buffer frame queue
    void ClearFrameQueue();

  signals:
    // The frame received event that passes the frame directly
    void FrameReceivedSignal( int status );


  private:
    // Since a Qt signal cannot contain a whole frame
    // the frame observer stores all FramePtr
    std::queue<AVT::VmbAPI::FramePtr> allFrames;
    QMutex allFramesMutex;



};

#endif
