#include "OpenCVCamera.h"

#include <opencv/highgui.h>
#include <opencv/cv.h>

using namespace plv;

OpenCVCamera::OpenCVCamera( int id ) :
    m_id( id ),
    m_state( CAM_UNINITIALIZED ),
    m_width( 0 ),
    m_height( 0 ),
    m_captureDevice( 0 )
{
}

OpenCVCamera::~OpenCVCamera()
{
    releaseCapture();
}

int OpenCVCamera::init()
{
    QMutexLocker lock( &m_opencv_mutex );

    m_captureDevice = cvCreateCameraCapture(0);
    if( m_captureDevice == 0 )
    {
        return -1;
    }

    // get width and height
    m_width  = (int) cvGetCaptureProperty( m_captureDevice, CV_CAP_PROP_FRAME_WIDTH );
    m_height = (int) cvGetCaptureProperty( m_captureDevice, CV_CAP_PROP_FRAME_HEIGHT );

    m_state = CAM_INITIALIZED;
    return 0;
}

void OpenCVCamera::run()
{
    assert( m_state != CAM_UNINITIALIZED );
    assert( m_captureDevice != 0 );

    while( m_state == CAM_RUNNING || m_state == CAM_PAUSED )
    {
        // get a frame, blocking call
        OpenCVImage* frame = getFrame();

        // send a signal to subscribers
        if( frame != 0 )
        {
            emit newFrame( frame );
        }

        if( m_state == CAM_PAUSED )
        {
            m_mutex.lock();

            // this will let this thread wait on the event
            // and unlocks the mutex so no deadlock is created
            m_condition.wait(&m_mutex);

            // we have woken up
            // mutex was relocked by the condition
            // unlock it here
            m_mutex.unlock();
        }
    }
}

void OpenCVCamera::start()
{
    QMutexLocker lock( &m_mutex );

    switch(m_state)
    {
    case CAM_UNINITIALIZED:
        // TODO throw exception?
        return;
    case CAM_INITIALIZED:
        // Start thread
        m_state = CAM_RUNNING;
        QThread::start();
    case CAM_RUNNING:
        // Do nothing
        return;
    case CAM_PAUSED:
        // Resume
        m_state = CAM_RUNNING;
        m_condition.wakeOne();
    }

}

OpenCVCamera::CameraState OpenCVCamera::getState() const
{
    return m_state;
}


void OpenCVCamera::pause()
{
    QMutexLocker lock( &m_mutex );

    if( m_state == CAM_RUNNING )
    {
        m_state = CAM_PAUSED;
    }
}

//void OpenCVCamera::stop()
//{
//    QMutexLocker lock( &m_mutex );
//
//    if( m_state == CAM_RUNNING )
//    {
//        m_state = CAM_STOPPING;
//    }
//    else if( m_state == CAM_PAUSED )
//    {
//        // this will wake the thread after this method has exited
//        m_condition.wakeOne();
//    }
//}

void OpenCVCamera::releaseCapture()
{
    QMutexLocker lock( &m_opencv_mutex );
    if(m_captureDevice !=0)
    {
        cvReleaseCapture(&m_captureDevice);
        m_captureDevice = 0;
    }
}

void OpenCVCamera::release()
{
    QMutexLocker lock( &m_mutex );

    CameraState current_state = m_state;
    m_state = CAM_UNINITIALIZED;

    switch(current_state)
    {
    case CAM_PAUSED:
        // this will wake the thread after m_mutex is released, unpausing it
        m_condition.wakeOne();
        m_mutex.unlock();
        m_mutex.lock();
        //fallthrough
    case CAM_RUNNING:
        // switching m_state to CAM_UNINITIALIZED
        // will cause the run loop to exit eventually.
        // wait for that here.
        QThread::wait();
        //fallthrough
    case CAM_INITIALIZED:
        // the thread is not running, so it's safe to release the capture device.
        releaseCapture();
        // fallthrough
    case CAM_UNINITIALIZED:
        return;
    }
}

OpenCVImage* OpenCVCamera::getFrame()
{
    IplImage* image = cvCloneImage(retrieveFrame());
    return new OpenCVImage(0, image);
}

const IplImage* OpenCVCamera::retrieveFrame()
{
    QMutexLocker lock( &m_opencv_mutex );

    if( m_state == CAM_UNINITIALIZED )
        throw "uninitialized";

    // first grab a frame, this is a fast function
    int status = cvGrabFrame( m_captureDevice );

    if( !status )
    {
        throw "failed to grab frame";
    }

    // now do post processing such as decompression
    const IplImage* image = cvRetrieveFrame( m_captureDevice );
    return image;



}