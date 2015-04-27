#ifndef CALIBDIALOG_H
#define CALIBDIALOG_H

#include <QDialog>

namespace Ui {
class CalibDialog;
}

class CalibDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibDialog(QWidget *parent = 0);
    ~CalibDialog();

private:
    Ui::CalibDialog *ui;
};

#endif // CALIBDIALOG_H
