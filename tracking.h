#ifndef TRACKING_H
#define TRACKING_H
#define MAXSUCCESSES 200
#define STEP 10

#include <QObject>
#include <QImage>
#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QWaitCondition>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <cmath>

using namespace cv;
using namespace std;

class Tracking : public QThread
{
    Q_OBJECT
public:
    //constructors
    explicit Tracking(QObject *parent = 0, int w = 9, int h = 6, double sq_size = 23.7);
    //Destructor
    ~Tracking();

private:
    bool stop;
    bool isNewFrame;
    bool isStopped();
    bool calibLoaded;
    bool calibMode;
    Mat RGBframe;
    Mat RGBframeUndist;
    Mat grayframe;
    Mat grayframeUndist;
    Mat intrinsic; //camera matrix
    Mat distCoeffs;

    Mat rvec;
    Mat tvec;
    vector<double> translation;
    vector<double> angles;
    vector<double> transStart;
    vector<double> angStart;

    QImage image;
    QMutex mutex;
    QWaitCondition waitcond;
    Size board_sz;
    vector<Point2f> corners; //temporarily hold the current snapshot's chessboard corners.
    vector<Point3f> obj;
    int numCornersVer; // Board width in squares - 1
    int numCornersHor; // Board height - 1
    int n_boards; // Number of boards
    int numSquares;
    float squareSize; //size of single square in mm
    void recompute(); //recompute variables after every change
    void loadCalib();
    void calibrate(); //calibrate the camera

protected:
    void  run(); //for starting the thread

signals:
    void sendFrame(const cv::Mat RGBframe);
    void showCalDial();
    void sendTR(std::vector<double> transTmp,
                std::vector<double> angTmp);
    void sendTmatrix(cv::Mat mat4);

public slots:
    void updateFrame(cv::Mat impFrame);
    void setRun();
    void setStop();
    void setCalibMode();
    void applyPrefs(int w, int h, double sz);
    void RTvecSetStart();
    void RTvecClear();



};

#endif // TRACKING_H
