#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_stopwatch=new Stopwatch(this);

    connect(ui->pb_startStop,&QPushButton::clicked,this,&MainWindow::slot_start_stop);
    connect(m_stopwatch,&Stopwatch::sig_send_time,this,&MainWindow::slot_update_time);
    connect(ui->pb_clear,&QPushButton::clicked,this,&MainWindow::slot_clear_stopwatch);
    connect(ui->pb_lap,&QPushButton::clicked,this,&MainWindow::slot_update_timelaps);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_start_stop(){
    if(ui->pb_startStop->text()=="Старт"){
        ui->pb_startStop->setText("Стоп");
        ui->pb_lap->setEnabled(true);
        m_stopwatch->start();
    }
    else{
        ui->pb_startStop->setText("Старт");
        ui->pb_lap->setEnabled(false);
        m_stopwatch->stop();
    }
}

void MainWindow::slot_update_time(double time){
    ui->lb_time->setText(QString("%1 сек").arg(time));
}

void MainWindow::slot_clear_stopwatch(){
    m_stopwatch->clear();
    slot_update_time(0);
    ui->txtB_timelaps->clear();
}

void MainWindow::slot_update_timelaps(){
    auto lap=m_stopwatch->get_lap();
    m_stopwatch->update_lap();
    ui->txtB_timelaps->append(QString("Круг %1, время: %2 сек").arg(lap.first).arg(lap.second));
}
