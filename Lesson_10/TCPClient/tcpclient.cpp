#include "tcpclient.h"

QDataStream &operator >>(QDataStream &out, ServiceHeader &data){

    out >> data.id;
    out >> data.idData;
    out >> data.status;
    out >> data.len;
    return out;
}

QDataStream &operator <<(QDataStream &in, ServiceHeader &data){

    in << data.id;
    in << data.idData;
    in << data.status;
    in << data.len;

    return in;
}

QDataStream &operator >>(QDataStream &out, StatServer &stats){

    out >> stats.incBytes;
    out >> stats.sendBytes;
    out >> stats.revPck;
    out >> stats.sendPck;
    out >> stats.workTime;
    out >> stats.clients;
    return out;
}


TCPclient::TCPclient(QObject *parent) : QObject(parent)
{

    socket=new QTcpSocket(this);
    connect(socket,&QTcpSocket::readyRead,this,&TCPclient::ReadyReed);
    connect(socket,&QTcpSocket::connected,this,[&]{
        emit sig_connectStatus(STATUS_SUCCES);
    });
    connect(socket,&QTcpSocket::errorOccurred,this,[&]{
        emit sig_connectStatus(ERR_CONNECT_TO_HOST);
    });
    connect(socket,&QTcpSocket::disconnected,this,&TCPclient::sig_Disconnected);

}

void TCPclient::SendRequest(ServiceHeader head)
{

    QByteArray sendHdr;
    QDataStream outStream(&sendHdr,QIODevice::WriteOnly);
    outStream << head;
    socket->write(sendHdr);

}

void TCPclient::SendData(ServiceHeader head, QString str)
{

    QByteArray sendData;
    QDataStream outStream(&sendData,QIODevice::WriteOnly);
    outStream << head << str;
    socket->write(sendData);

}

void TCPclient::ConnectToHost(QHostAddress host, uint16_t port)
{

    socket->connectToHost(host,port);

}

void TCPclient::DisconnectFromHost()
{

    socket->disconnectFromHost();

}


void TCPclient::ReadyReed()
{

    QDataStream incStream(socket);

    if(incStream.status() != QDataStream::Ok){
        QMessageBox msg;
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Ошибка открытия входящего потока для чтения данных!");
        msg.exec();
    }

    while(incStream.atEnd() == false){
        if(servHeader.idData == 0){
            if(socket->bytesAvailable() < sizeof(ServiceHeader)){
                return;
            }
            incStream >> servHeader;
            if(servHeader.id != ID){
                uint16_t hdr = 0;
                while(incStream.atEnd()){
                    incStream >> hdr;
                    if(hdr == ID){
                        incStream >> servHeader.idData;
                        incStream >> servHeader.status;
                        incStream >> servHeader.len;
                        break;
                    }
                }
            }
        }
        if(socket->bytesAvailable() < servHeader.len){
            return;
        }
        else{
            ProcessingData(servHeader, incStream);
            servHeader.idData = 0;
            servHeader.status = 0;
            servHeader.len = 0;
        }
    }

}

void TCPclient::ProcessingData(ServiceHeader header, QDataStream &stream)
{

    if(header.status!=STATUS_SUCCES){
        emit sig_Error(header.status);
        return;
    }

    switch (header.idData){

        case GET_TIME:
        {
            QDateTime time;
            stream >> time;
            emit sig_sendTime(time);
            break;
        }
        case GET_SIZE:
        {
            uint32_t size;
            stream >> size;
            emit sig_sendFreeSize(size);
            break;
        }
        case GET_STAT:
        {
            StatServer stats;
            stream >> stats;
            emit sig_sendStat(stats);
            break;
        }
        case SET_DATA:
        {
            QString str;
            stream >> str;
            emit sig_SendReplyForSetData(str);
            break;
        }
        case CLEAR_DATA:
            emit sig_Success(header.idData);
            break;
        default:
            return;

        }

}
