#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345
#define MSG_PORT 56789

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void ReadDatagram( QNetworkDatagram datagram);
    void SendDatagram(QByteArray data );

    void ReadMessageDatagram(QNetworkDatagram datagram);
    void SendMessageDatagram(QByteArray data);


private slots:
    void readPendingDatagrams(void);

private:
    QUdpSocket* serviceUdpSocket;
    QUdpSocket* messagesUdpSocket;

signals:
    void sig_sendTimeToGUI(QDateTime data);
    void sig_sendMessageInfo(QString,qsizetype);

};

#endif // UDPWORKER_H
