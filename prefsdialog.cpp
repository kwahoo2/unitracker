#include "prefsdialog.h"
#include "ui_prefsdialog.h"

PrefsDialog::PrefsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrefsDialog)
{
    width = 9;
    height = 6;
    size = 23.0;
    ui->setupUi(this);

    QObject::connect(this->ui->spinBoxW, SIGNAL(valueChanged(int)),
                     this, SLOT(setW(int)));
    QObject::connect(this->ui->spinBoxH, SIGNAL(valueChanged(int)),
                     this, SLOT(setH(int)));
    QObject::connect(this->ui->doubleSpinBoxSz, SIGNAL(valueChanged(double)),
                     this, SLOT(setSz(double)));
    QObject::connect(this->ui->spinBoxCam, SIGNAL(valueChanged(int)),
                     this, SLOT(setCam(int)));
    QObject::connect(this->ui->portApplyButton, SIGNAL(clicked()),
                     this, SLOT(setPort()));
    loadSettings();

}

PrefsDialog::~PrefsDialog()
{
    delete ui;
}

void PrefsDialog::setW(int val)
{
    width = val - 1; //number of cornes = number of squares -1
    settings.setValue("Tracking/chessboard_width", width);
    emit sendPrefs(width, height, size);
}
void PrefsDialog::setH(int val)
{
    height = val - 1;
    settings.setValue("Tracking/chessboard_height", height);
    emit sendPrefs(width, height, size);
}
void PrefsDialog::setSz(double val)
{
    size = val;
    settings.setValue("Tracking/square_size", size);
    emit sendPrefs(width, height, size);
}
void PrefsDialog::setCam(int cam)
{
    settings.setValue("Capture/camera", cam);
    emit sendCam(cam);
}
void PrefsDialog::setPort()
{
    port = ui->lineEditPort->text().toShort();
    settings.setValue("Network/port", port);
    emit sendPort(port);
}
void PrefsDialog::loadSettings()
{
    settings.beginGroup("Tracking");
        if (settings.contains("chessboard_width") &&
                settings.contains("chessboard_height") &&
                settings.contains("chessboard_size"))
            {
                width = settings.value("chessboard_width").toInt();
                height = settings.value("chessboard_height").toInt();
                size = settings.value("square_size").toDouble();
                emit sendPrefs(width, height, size);
                ui->spinBoxW->setValue(width+1);
                ui->spinBoxH->setValue(height+1);
                ui->doubleSpinBoxSz->setValue(size);
            }
    settings.endGroup();

    if (settings.contains("Capture/camera"))
    {
        int camera = settings.value("Capture/camera").toInt();
        emit sendCam(camera);
        ui->spinBoxCam->setValue(camera);
    }

    if (settings.contains("Network/port"))
    {
        port = settings.value("Network/port").toUInt();
        emit sendPort(port);
        ui->lineEditPort->setText(QString::number(port));
    }
}
