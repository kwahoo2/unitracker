#include "tracking.h"

Tracking::Tracking(QObject *parent, int w, int h, double sq_size) :
    QThread(parent)
{

    numCornersVer = w; // Board width in squares - 1
    numCornersHor = h; // Board height - 1
    squareSize = sq_size;
    rvec = Mat::zeros(3, 1, CV_64F); //important: solvePnP output is CV_64F
    tvec = Mat::zeros(3, 1, CV_64F);
    stop = true;
    isNewFrame = false;
    intrinsic = Mat(3, 3, CV_32FC1);
    calibLoaded = false;
    calibMode = false;
    RTvecClear();
    recompute();
    loadCalib();

}
void Tracking::setRun()
{

    if (!isRunning())
    {
        if (isStopped())
        { //isStopped return stop var value
            stop = false;
        }
        start(LowPriority);
        qDebug() << "Tracking thread started";
    }
}
void Tracking::run()
{

    while(!stop)
    {
        if((!calibLoaded) && (!calibMode))
        {
            emit showCalDial();
            stop = true;
        }
        if (calibMode) calibrate();

        if (!RGBframe.empty())
            {
            undistort(RGBframe, RGBframeUndist, intrinsic, distCoeffs);
            cvtColor(RGBframeUndist, grayframeUndist, CV_RGB2GRAY);
            vector<Point3f> objtmp = obj; //avoid corners-obj incosistency while changing preferences
            bool found = findChessboardCorners(grayframeUndist, board_sz, corners,
                                                    CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
            if (found)
            {

                drawChessboardCorners(RGBframeUndist, board_sz, corners, found);
                solvePnP(objtmp, corners, intrinsic, distCoeffs, rvec, tvec); //find chessboard position

                /*conversion to std::*/
                translation.assign((double*)tvec.datastart, (double*)tvec.dataend);

                Mat rodrigMat;
                Rodrigues(rvec, rodrigMat); //conversion rotation vector to matrix

                angles[0] = asin(rodrigMat.at<double>(2,1)) * 180 / M_PI; //row major
                angles[1] = asin(rodrigMat.at<double>(0,2)) * 180 / M_PI;
                angles[2] = asin(rodrigMat.at<double>(1,0)) * 180 / M_PI;

                vector<double> transTmp = {0.0, 0.0, 0.0};
                vector<double> angTmp = {0.0, 0.0, 0.0};

                for (int i = 0; i < 3; i++)                    //incremental measurment
                {
                    transTmp[i] = translation[i] - transStart[i];
                    angTmp[i] = angles[i] - angStart[i];
                }

                emit sendTR(transTmp,
                            angTmp);
                /*      r11    r12     r13  0
                 *      r21    r22     r23  0
                 * R =  r31    r32     r33  0
                 *      0      0       0    1
                 *
                 *      1   0   0   X
                 *      0   1   0   Y
                 * T =  0   0   1   Z
                 *      0   0   0   1
                 *
                 * R x T = Tr
                 *
                 */

                Mat rot4 = Mat::eye(4, 4, CV_64F);//diagonal 1
                Mat tr4 = Mat::eye(4, 4, CV_64F);
                rodrigMat.copyTo(rot4(Rect(0, 0, 3, 3)));
                tvec.copyTo(tr4(Rect(3, 0, 1, 3))); //start column, start row, width, height

                Mat transf4 = rot4 * tr4;
                emit sendTmatrix(transf4);

                //cout << "rodrigMat" << rodrigMat << endl;
                //cout << "rot4d" << rot4 << endl;
                //cout << "tvec" << tvec << endl;
                //cout << "tr4d" << tr4 << endl;
                //cout << "Tr" << transf4 << endl;
            }
            emit sendFrame(RGBframeUndist);
        }
    }

}
void Tracking::updateFrame(cv::Mat impFrame)
{
    RGBframe = impFrame;
    isNewFrame = true;
}
void Tracking::loadCalib()
{
    /*try open the file and read calibration data*/
    FileStorage fs("calibration.yml", FileStorage::READ);
    if(fs.isOpened())
    {
        fs["intrinsic"] >> intrinsic;
        fs["distCoeffs"] >> distCoeffs;
        calibLoaded = true;
    }
    fs.release();
}

void Tracking::calibrate()
{
    vector<vector<Point3f> > object_points; //physical position of the corners (in 3D space).
    vector<vector<Point2f> > image_points; //location of the corners on in the image (in 2 dimensions).
    int successes=0;
    while ((!stop) && (successes < MAXSUCCESSES))
    {
        if (isNewFrame)
        {
            qDebug() << "calibration";
            cvtColor(RGBframe, grayframe, CV_RGB2GRAY);
            isNewFrame = false;
            bool found = findChessboardCorners( grayframe, board_sz, corners,
                                        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS );

            if (found)
            {
                drawChessboardCorners(RGBframe, board_sz, corners, found);
                if (successes % STEP == 0) //give some time to move chessboard
                {
                    image_points.push_back(corners);
                    object_points.push_back(obj);
                }


                successes++;
            }
            emit sendFrame(RGBframe);
        }

    }
    vector<Mat> rvecs;
    vector<Mat> tvecs;
    intrinsic.ptr<float>(0)[0] = 1;
    intrinsic.ptr<float>(1)[1] = 1;
    calibrateCamera(object_points, image_points, grayframe.size(), intrinsic, distCoeffs, rvecs, tvecs);
    //cout << intrinsic << distCoeffs;


    /*save to file*/
    FileStorage fs("calibration.yml", FileStorage::WRITE);
    fs << "intrinsic" << intrinsic << "distCoeffs" << distCoeffs;
    fs.release();
    calibLoaded = true;
    calibMode = false;
}

void Tracking::recompute()
{

    numSquares = numCornersHor * numCornersVer;
    board_sz = Size(numCornersHor, numCornersVer);
    obj.clear();
    for(int j=0;j<numSquares;j++)
        obj.push_back(Point3f(j/numCornersHor*squareSize, j%numCornersHor*squareSize, 0.0));
}

Tracking::~Tracking()
{
    stop = true;
}
bool Tracking::isStopped()
{
    return this->stop;
}
void Tracking::setStop()
{
    stop = true;
}
void Tracking::setCalibMode()
{
    calibMode = true;

}
void Tracking::applyPrefs(int w, int h, double sz)
{
    numCornersVer = w; // Board width in squares - 1
    numCornersHor = h; // Board height - 1
    squareSize = sz;
    recompute();
    qDebug() << "Preferences updated";
}
void Tracking::RTvecSetStart()
{
  transStart.swap(translation);   //starting pos for incremental measurment
  angStart.swap(angles);
}

void Tracking::RTvecClear()
{
    translation = {0.0, 0.0, 0.0};
    angles = {0.0, 0.0, 0.0};
    transStart = {0.0, 0.0, 0.0};
    angStart = {0.0, 0.0, 0.0};
}


