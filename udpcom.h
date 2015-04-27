#ifndef UDPCOM_H
#define UDPCOM_H

#include <QObject>
#include <QUdpSocket>
#include <QDebug>
#include <opencv2/core/core.hpp>

class Udpcom : public QObject
{
    Q_OBJECT
public:
    explicit Udpcom(QObject *parent = 0);
    ~Udpcom();

signals:

public slots:
    void sendTRDatagram(cv::Mat mat4);
    void setPort(quint16 p);


private:
    QUdpSocket *udpSocket;
    quint16 port;

};

#endif // UDPCOM_H
