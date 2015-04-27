#include "calibdialog.h"
#include "ui_calibdialog.h"

CalibDialog::CalibDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibDialog)
{
    ui->setupUi(this);
    QObject::connect(ui->CloseButton, SIGNAL(clicked()),
                     this, SLOT(accept()));
}

CalibDialog::~CalibDialog()
{
    delete ui;
}
