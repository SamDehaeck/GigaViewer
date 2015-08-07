#include <vimbaframeobserver.h>
#include "cambackend.h"

using namespace AVT::VmbAPI;

VimbaFrameObserver::VimbaFrameObserver(CameraPtr pCamera, CamBackend *consumer) : IFrameObserver( pCamera ) {
    connect(this,SIGNAL(FrameReceivedSignal(int)),consumer,SLOT(GrabFrame()));
}


void VimbaFrameObserver::FrameReceived( const FramePtr pFrame )
{
    bool bQueueDirectly = true;
    VmbFrameStatusType eReceiveStatus;

    if( VmbErrorSuccess == pFrame->GetReceiveStatus( eReceiveStatus ) )
    {
        // Lock the frame queue
        allFramesMutex.lock();
        // Add frame to queue
        allFrames.push( pFrame );
        // Unlock frame queue
        allFramesMutex.unlock();
        // Emit the frame received signal
        emit FrameReceivedSignal( eReceiveStatus );
        bQueueDirectly = false;
    }

    // If any error occurred we queue the frame without notification
    if( true == bQueueDirectly )
    {
        m_pCamera->QueueFrame( pFrame ); //m_pCamera is the inherent name of the camera pointer of an IFrameObserver
    }
}

// Returns the oldest frame that has not been picked up yet
FramePtr VimbaFrameObserver::GetFrame()
{
    // Lock the frame queue
    allFramesMutex.lock();
    // Pop frame from queue
    FramePtr res = allFrames.front();
    allFrames.pop();
    // Unlock frame queue
    allFramesMutex.unlock();
    return res;
}

void VimbaFrameObserver::ClearFrameQueue()
{
    // Lock the frame queue
    allFramesMutex.lock();
    // Clear the frame queue and release the memory
    std::queue<FramePtr> empty;
    std::swap( allFrames, empty );
    // Unlock the frame queue
    allFramesMutex.unlock();
}
