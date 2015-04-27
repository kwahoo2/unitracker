#include "mainwindow.h"
#include "ui_mainwindow.h"

Q_DECLARE_METATYPE(cv::Mat)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    myDisplay = new Display(); //create pointer display object
    myTracking = new Tracking();
    myUdp = new Udpcom();
    myCalDial = new CalibDialog(this); //a window showed before calibration
    myCalDial->setModal(true);
    myPrefsDial = new PrefsDialog(this); //dialog with options
    myPrefsDial->setModal(false);

    QObject::connect(myDisplay, SIGNAL(showFrame(QImage)),
                     this, SLOT(updateDisplayUI(QImage))); //link necessary for image update*/

    QObject::connect(myTracking, SIGNAL(showCalDial()),
                    this, SLOT(on_actionCalibrate_triggered()));

    QObject::connect(myCalDial, SIGNAL(accepted()),
                     this, SLOT(startCalib()));

    QObject::connect(myTracking, SIGNAL(sendTR(std::vector<double>,
                                               std::vector<double>)),
                     this, SLOT(updateTRlabels(std::vector<double>,
                                               std::vector<double>)));
    QObject::connect(myTracking, SIGNAL(sendTmatrix(cv::Mat)),
                     myUdp, SLOT(sendTRDatagram(cv::Mat)));

/*preferences support*/
    QObject::connect(myPrefsDial, SIGNAL(sendPrefs(int,int,double)),
                     myTracking, SLOT(applyPrefs(int,int,double)));

    QObject::connect(myPrefsDial, SIGNAL(sendCam(int)),
                     myDisplay, SLOT(setCamera(int)));

    QObject::connect(myPrefsDial, SIGNAL(sendPort(quint16)),
                     myUdp, SLOT(setPort(quint16)));

    qRegisterMetaType< cv::Mat >("cv::Mat"); //regiter typedef for moc
    qRegisterMetaType< std::vector<double> >("std::vector<double>");

    ui->setupUi(this);


    /*main window butons, have to be later than setupUi*/
    QObject::connect(this->ui->setButton, SIGNAL(clicked()),
                     myTracking, SLOT(RTvecSetStart()));

    QObject::connect(this->ui->resButton, SIGNAL(clicked()),
                     myTracking, SLOT(RTvecClear()));

}

MainWindow::~MainWindow()
{
    myTracking->setStop();
    myDisplay->setStop();
    QThread::msleep(500); //give some time to stop threads
    delete ui;
    delete myDisplay;
    delete myTracking;
    delete myUdp;
    //delete myCalDial; //not necessary because (this) arg
}

void MainWindow::updateDisplayUI(QImage img)
{
    if (!img.isNull())
    {
        ui->view1->setAlignment(Qt::AlignCenter);
        ui->view1->setPixmap(QPixmap::fromImage(img).scaled(ui->view1->size(),
                                           Qt::KeepAspectRatio, Qt::FastTransformation));

    }
}

void MainWindow::on_actionCapture_only_triggered()
{
    QObject::disconnect(myDisplay, SIGNAL(sendFrame(cv::Mat)), //update frame for tracking
                     myTracking, SLOT(updateFrame(cv::Mat)));

    QObject::disconnect(myTracking, SIGNAL(sendFrame(cv::Mat)), //send and convert to QImage
                     myDisplay, SLOT(convertShow(cv::Mat)));

    QObject::connect(myDisplay, SIGNAL(sendFrame(cv::Mat)),
                     myDisplay, SLOT(convertShow(cv::Mat)));

    myTracking->setStop();
    myDisplay->setRun();
    ui->actionCapture_only->setDisabled(true);
    ui->actionStart_tracking->setEnabled(true);
}

void MainWindow::on_actionStart_tracking_triggered()
{
    QObject::disconnect(myDisplay, SIGNAL(sendFrame(cv::Mat)),
                     myDisplay, SLOT(convertShow(cv::Mat)));

    QObject::connect(myDisplay, SIGNAL(sendFrame(cv::Mat)), //update frame for tracking
                     myTracking, SLOT(updateFrame(cv::Mat)));

    QObject::connect(myTracking, SIGNAL(sendFrame(cv::Mat)), //send and convert to QImage
                     myDisplay, SLOT(convertShow(cv::Mat)));

    myDisplay->setRun();
    myTracking->setRun();
    ui->actionStart_tracking->setDisabled(true);
    ui->actionCapture_only->setEnabled(true);
}

void MainWindow::on_actionStop_all_triggered()
{
    myDisplay->setStop();
    myTracking->setStop();
    ui->actionStart_tracking->setEnabled(true);
    ui->actionCapture_only->setEnabled(true);
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionCalibrate_triggered()
{
    myCalDial->show();
    ui->actionStart_tracking->setEnabled(true);
    ui->actionCapture_only->setEnabled(true);
}
void MainWindow::startCalib()
{
    QObject::disconnect(myDisplay, SIGNAL(sendFrame(cv::Mat)),
                     myDisplay, SLOT(convertShow(cv::Mat)));

    QObject::connect(myDisplay, SIGNAL(sendFrame(cv::Mat)), //update frame for tracking
                     myTracking, SLOT(updateFrame(cv::Mat)));

    QObject::connect(myTracking, SIGNAL(sendFrame(cv::Mat)), //send and convert to QImage
                     myDisplay, SLOT(convertShow(cv::Mat)));


    myDisplay->setRun();
    myTracking->setRun();
    myTracking->setCalibMode();
}
void MainWindow::updateTRlabels(std::vector<double> translation,
                                std::vector<double> angles)
{
    QString qstr = tr("Translation: X ") + QString::number(translation[0]) + " Y " +
            QString::number(translation[1]) + " Z " + QString::number(translation[2]);

    ui->tlabel->setText(qstr);

    QString qstr2 = tr("Angles: ") + QString::number(angles[0]) + " " +
            QString::number(angles[1]) + " " + QString::number(angles[2]);

    ui->rlabel->setText(qstr2);
}


void MainWindow::on_actionPrefs_triggered()
{
    myPrefsDial->show();
}
