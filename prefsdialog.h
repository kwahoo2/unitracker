#ifndef PREFSDIALOG_H
#define PREFSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class PrefsDialog;
}

class PrefsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrefsDialog(QWidget *parent = 0);
    ~PrefsDialog();

private:
    Ui::PrefsDialog *ui;
    int width;
    int height;
    double size;
    quint16 port;
    QSettings settings;
    void loadSettings();

private slots:
    void setW(int val);
    void setH(int val);
    void setSz(double val);
    void setCam(int val);
    void setPort();

signals:
   void sendPrefs(const int w, const int h, const double sz);
   void sendCam(int cam);
   void sendPort(quint16 p);

};

#endif // PREFSDIALOG_H
