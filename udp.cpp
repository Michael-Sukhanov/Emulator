#include "udp.h"
#include "IP_bus.h"

UDP::UDP(QObject *parent) :
    QObject(parent)
{
    socket= new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 40001);
    socket->connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
}

void UDP::readyRead()
{
    // when data comes in
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    // qint64 QUdpSocket::readDatagram(char * data, qint64 maxSize,
    //                 QHostAddress * address = 0, quint16 * port = 0)
    // Receives a datagram no larger than maxSize bytes and stores it in data.
    // The sender's host address and port is stored in *address and *port
    // (unless the pointers are 0).

    socket->readDatagram(buffer.data(), buffer.size(),
                         &sender, &senderPort);

    qDebug() << "Buffer: " << buffer.toHex();
    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug() << "Message: " << buffer.toHex();
    qDebug() << "Size: "<<buffer.size();
    qDebug() << Packet_Header_Analyse(buffer);
    qDebug() << Get_Transaction_Type(buffer.mid(7,1));
    qDebug() << Number_of_Words(buffer.mid(6,1));
    qDebug() << Get_Transaction_ID(buffer.mid(4,2));

}

enum PacketType UDP::Packet_Header_Analyse(QByteArray data)
{
   quint32 Header;
   quint16 ID;
   QDataStream stream_id(data.mid(1,1));
   stream_id >> ID;
   QDataStream stream(data);
   stream >> Header;
    enum PacketType type;
    if (Header==PacketHeader(status, ID))
        type=status;
    else if (Header==PacketHeader(control, ID))
        type=control;
    else if (Header==PacketHeader(resend, ID))
        type=resend;
    else
        type = not_defined;
    return type;

    }

enum TransactionType UDP::Get_Transaction_Type(QByteArray cuted_data)
{
    quint8 Type_ID_and_Infocode;
    QDataStream stream_Type_ID_and_Infocode(cuted_data);
    stream_Type_ID_and_Infocode >> Type_ID_and_Infocode;
    enum TransactionType type;
    switch (Type_ID_and_Infocode) {
    case 0x0f: type = read;
        break;
    case 0x1f: type = write;
        break;
    case 0x2f: type = nonIncrementingRead;
        break;
    case 0x3f: type = nonIncrementingWrite;
        break;
    case 0x4f: type = RMWbits;
        break;
    case 0x5f: type = RMWsum;
        break;
    case 0x6f: type = configurationRead;
        break;
    case 0x7f: type = configurationWrite;
        break;
    default: type = Not_defined;


    }

    return type;
}

quint8 UDP::Number_of_Words(QByteArray cuted_data)
{
    quint8 Number_of_words;
    QDataStream stream_for_num_of_words(cuted_data);
    stream_for_num_of_words >> Number_of_words;
    return Number_of_words;
}

quint16 UDP::Get_Transaction_ID(QByteArray cuted_data)
{
    quint16 Trans_ID;
    QDataStream stream_transaction_id(cuted_data);
    stream_transaction_id >> Trans_ID;
    return Trans_ID & 0x0fff;
}




