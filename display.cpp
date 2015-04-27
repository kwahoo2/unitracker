#include "display.h"

Display::Display(QObject *parent) :
    QThread(parent)
{
    camera = 0;
    stop = true;
}

void Display::setRun()
{
    if (!isRunning()) {
        if (isStopped()){ //isStopped return stop var value
            stop = false;
        }
        start(LowPriority);
        qDebug() << "Camera thread started";

    }
}
void Display::run()
{
    capture.open(camera); //open a camera

    if( capture.isOpened() )
    {
        while(!stop)
        {
            capture >> frame;
            cvtColor(frame, RGBframe, CV_BGR2RGB);
            emit sendFrame(RGBframe); //emit frame for tracking
        }
        capture.release(); //shut down the camera
    }

}

void Display::convertShow(cv::Mat RGBtemp)
{
    image = QImage((const unsigned char*)(RGBtemp.data),
                      RGBtemp.cols,RGBtemp.rows,QImage::Format_RGB888);

    emit showFrame(image); //signal to show processed image
}

bool Display::isStopped()
{
    return this->stop;
}
void Display::setStop()
{
    stop = true;
}
void Display::setCamera(int cam) //not tested
{
    stop = true;
    while (isRunning())
    {

    }
    if (isStopped()){ //isStopped return stop var value
        stop = false;
        camera = cam;
    }
    start(LowPriority);

}

Display::~Display()
{
    stop = true;
}
