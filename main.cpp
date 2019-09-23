#include <QCoreApplication>
#include <QtNetwork/QUdpSocket>
#include "udp.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    UDP obj;
    return a.exec();
}
