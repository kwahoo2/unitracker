#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMetaType>
#include <QDebug>
#include "display.h"
#include "tracking.h"
#include "calibdialog.h"
#include "prefsdialog.h"
#include "udpcom.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CalibDialog *myCalDial;
    Display *myDisplay;
    Tracking *myTracking;
    PrefsDialog *myPrefsDial;
    Udpcom *myUdp;

private slots:
    //Display captured frame in player UI
    void updateDisplayUI(QImage img);
    void on_actionCapture_only_triggered();
    void on_actionStart_tracking_triggered();
    void on_actionStop_all_triggered();
    void on_actionExit_triggered();
    void on_actionPrefs_triggered();
    void on_actionCalibrate_triggered();
    void startCalib();
    void updateTRlabels(std::vector<double> translation,
                        std::vector<double> angles);


};

#endif // MAINWINDOW_H
