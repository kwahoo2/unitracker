#ifndef DISPLAY_H
#define DISPLAY_H

#include <QObject>
#include <QImage>
#include <QThread>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "tracking.h"

using namespace cv;
using namespace std;

class Display : public QThread
{
    Q_OBJECT
public:
    //Constructor
    explicit Display(QObject *parent = 0);
    //Destructor
    ~Display();

private:
    bool stop;
    bool isStopped();
    int camera;
    QImage image;
    Mat frame;
    Mat RGBframe;
    VideoCapture capture; //camera capture object

protected:
    void run();

signals:
    void showFrame(const QImage &img);
    void sendFrame(const cv::Mat RGBframe);

public slots:
    void setRun();
    void setStop();
    void convertShow(cv::Mat RGBtemp);
    void setCamera(int cam);

};

#endif // DISPLAY_H
