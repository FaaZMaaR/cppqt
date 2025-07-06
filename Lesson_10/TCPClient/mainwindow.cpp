#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client = new TCPclient(this);

    ui->le_data->setEnabled(false);
    ui->pb_request->setEnabled(false);
    ui->lb_connectStatus->setText("Отключено");
    ui->lb_connectStatus->setStyleSheet("color: red");

    connect(client, &TCPclient::sig_Disconnected, this, [&]{
        ui->lb_connectStatus->setText("Отключено");
        ui->lb_connectStatus->setStyleSheet("color: red");
        ui->pb_connect->setText("Подключиться");
        ui->le_data->setEnabled(false);
        ui->pb_request->setEnabled(false);
        ui->spB_port->setEnabled(true);
        ui->spB_ip1->setEnabled(true);
        ui->spB_ip2->setEnabled(true);
        ui->spB_ip3->setEnabled(true);
        ui->spB_ip4->setEnabled(true);
    });

    connect(client,&TCPclient::sig_connectStatus,this,&MainWindow::DisplayConnectStatus);
    connect(client,&TCPclient::sig_Error,this,&MainWindow::DisplayError);
    connect(client,&TCPclient::sig_sendTime,this,&MainWindow::DisplayTime);
    connect(client,&TCPclient::sig_sendFreeSize,this,&MainWindow::DisplayFreeSpace);
    connect(client,&TCPclient::sig_sendStat,this,&MainWindow::DisplayStat);
    connect(client,&TCPclient::sig_SendReplyForSetData,this,&MainWindow::SetDataReply);
    connect(client,&TCPclient::sig_Success,this,&MainWindow::DisplaySuccess);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DisplayTime(QDateTime time)
{
    ui->tb_result->append(QString("Текущее время и дата на сервере: %1").arg(time.toString()));
}
void MainWindow::DisplayFreeSpace(uint32_t freeSpace)
{
    ui->tb_result->append(QString("Свободное место на сервере: %1 байт").arg(freeSpace));
}
void MainWindow::SetDataReply(QString replyString)
{
    ui->tb_result->append(QString("На сервер успешно передана строка \"%1\"").arg(replyString));
}
void MainWindow::DisplayStat(StatServer stat)
{
    ui->tb_result->append(QString("Статистика сервера:"
                                  "\nКоличество принятых байт: %1"
                                  "\nКоличество переданных байт: %2"
                                  "\nКоличество принятых пакетов: %3"
                                  "\nКоличество переданных пакетов: %4"
                                  "\nВремя работы сервера, сек: %5"
                                  "\nКоличество подключенных клиентов: %6")
                              .arg(stat.incBytes).arg(stat.sendBytes).arg(stat.revPck).arg(stat.sendPck).arg(stat.workTime).arg(stat.clients));
}
void MainWindow::DisplayError(uint16_t error)
{
    switch (error) {
    case ERR_NO_FREE_SPACE:
        ui->tb_result->append("Ошибка! Нехватает места на сервере!");
        break;
    case ERR_ZERO_LEN:
        ui->tb_result->append("Ошибка! Отправлены пустые данные!");
        break;
    case ERR_NO_FUNCT:
        ui->tb_result->append("Ошибка! Данная функция не реализована!");
        break;
    default:
        break;
    }
}

void MainWindow::DisplaySuccess(uint16_t typeMess)
{
    switch (typeMess) {
    case CLEAR_DATA:
        ui->tb_result->append("Место на сервере успешно очищено");
        break;
    default:
        break;
    }

}

void MainWindow::DisplayConnectStatus(uint16_t status)
{

    if(status == ERR_CONNECT_TO_HOST){

        ui->tb_result->append("Ошибка подключения к порту: " + QString::number(ui->spB_port->value()));

    }
    else{
        ui->lb_connectStatus->setText("Подключено");
        ui->lb_connectStatus->setStyleSheet("color: green");
        ui->pb_connect->setText("Отключиться");
        ui->spB_port->setEnabled(false);
        ui->pb_request->setEnabled(true);
        ui->spB_ip1->setEnabled(false);
        ui->spB_ip2->setEnabled(false);
        ui->spB_ip3->setEnabled(false);
        ui->spB_ip4->setEnabled(false);
    }

}

void MainWindow::on_pb_connect_clicked()
{
    if(ui->pb_connect->text() == "Подключиться"){

        uint16_t port = ui->spB_port->value();

        QString ip = ui->spB_ip4->text() + "." +
                     ui->spB_ip3->text() + "." +
                     ui->spB_ip2->text() + "." +
                     ui->spB_ip1->text();

        client->ConnectToHost(QHostAddress(ip), port);

    }
    else{

        client->DisconnectFromHost();
    }
}

void MainWindow::on_pb_request_clicked()
{

   ServiceHeader header;

   header.id = ID;
   header.status = STATUS_SUCCES;
   header.len = 0;

   switch (ui->cb_request->currentIndex()){

       //Получить время
       case 0:
           header.idData=GET_TIME;
           break;
       //Получить свободное место
       case 1:
           header.idData=GET_SIZE;
           break;
       //Получить статистику
       case 2:
           header.idData=GET_STAT;
           break;
       //Отправить данные
       case 3:
           header.idData=SET_DATA;
           header.len=ui->le_data->text().size();
           client->SendData(header,ui->le_data->text());
           return;
       //Очистить память на сервере
       case 4:
           header.idData=CLEAR_DATA;
           break;
       default:
       ui->tb_result->append("Такой запрос не реализован в текущей версии");
       return;

   }

   client->SendRequest(header);

}

void MainWindow::on_cb_request_currentIndexChanged(int index)
{

    if(ui->cb_request->currentIndex() == 3){
        ui->le_data->setEnabled(true);
    }
    else{
        ui->le_data->setEnabled(false);
    }

}

