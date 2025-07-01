#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{



}


/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket()
{

    serviceUdpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);

    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);

    messagesUdpSocket = new QUdpSocket(this);
    messagesUdpSocket->bind(QHostAddress::LocalHost, MSG_PORT);
    connect(messagesUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);

}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{

    QByteArray data;
    data = datagram.data();


    QDataStream inStr(&data, QIODevice::ReadOnly);
    QDateTime dateTime;
    inStr >> dateTime;

    emit sig_sendTimeToGUI(dateTime);
}
/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendDatagram(QByteArray data)
{
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */
    while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }

    while(messagesUdpSocket->hasPendingDatagrams()){
        QNetworkDatagram datagram = messagesUdpSocket->receiveDatagram();
        ReadMessageDatagram(datagram);
    }

}

void UDPworker::ReadMessageDatagram(QNetworkDatagram datagram)
{

    QByteArray data;
    data = datagram.data();
    QHostAddress messageSender = datagram.senderAddress();
    QDataStream inStr(&data, QIODevice::ReadOnly);
    QString message;
    inStr >> message;

    emit sig_sendMessageInfo(messageSender.toString(),message.size());

}

void UDPworker::SendMessageDatagram(QByteArray data)
{

    messagesUdpSocket->writeDatagram(data, QHostAddress::LocalHost, MSG_PORT);

}
