#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorker->SendDatagram(dataToSend);
        timer->start(TIMER_DELAY);

    });

    connect(ui->pb_sendMessage,&QPushButton::clicked,this,&MainWindow::slot_send_message);
    connect(ui->le_sendMessage,&QLineEdit::returnPressed,this,&MainWindow::slot_send_message);
    connect(udpWorker,&UDPworker::sig_sendMessageInfo,this,&MainWindow::slot_display_message_info);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
}


void MainWindow::DisplayTime(QDateTime data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". "
                                                                "Принято пакетов " + QString::number(counterPck));


}


void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
}

void MainWindow::slot_send_message(){

    QString str_message = ui->le_sendMessage->text();
    QByteArray ba_message;
    QDataStream outStr(&ba_message,QIODevice::WriteOnly);
    outStr << str_message;
    udpWorker->SendMessageDatagram(ba_message);

}

void MainWindow::slot_display_message_info(QString address, qsizetype messageSize){
    ui->te_result->append(QString("Принято сообщение от %1, размер сообщения (байт) - %2").arg(address).arg(messageSize));
}
