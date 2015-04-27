#include "udpcom.h"

Udpcom::Udpcom(QObject *parent) :
    QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    port = 7755;
}
void Udpcom::sendTRDatagram(cv::Mat mat4)
{
    QByteArray datagram;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            double mval = mat4.at<double>(i, j);
            datagram.append(reinterpret_cast<const char*>(&mval), sizeof(mval));
        }
    }


    udpSocket->writeDatagram(datagram, QHostAddress::LocalHost, port);
}

void Udpcom::setPort(quint16 p)
{
    port = p;
    qDebug() << "Port is: " << port;
}

Udpcom::~Udpcom()
{

}
/*python receiver code
import socket
from struct import unpack

UDP_IP = "127.0.0.1"
UDP_PORT = 7755

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    #print "received message:", data
    du = unpack('dddddddddddddddd', data) #4x4 matrix - 16 doubles
    print du
*/
