#ifndef UDP_H
#define UDP_H

#include <QObject>
#include <QtNetwork/QUdpSocket>
#include "IP_bus.h"

class UDP : public QObject
{
    Q_OBJECT
public:
    explicit UDP(QObject *parent = nullptr);
    //void HelloUDP();

signals:

public slots:

    void readyRead();

private:
    QUdpSocket* socket;

    enum PacketType Packet_Header_Analyse(QByteArray data);

    //QByteArray Get_Packet_id(QByteArray data);

    enum TransactionType Get_Transaction_Type(QByteArray cuted_data);

    quint8 Number_of_Words(QByteArray cuted_data);

    quint16 Get_Transaction_ID(QByteArray cuted_data);



};


#endif // UDP_H
