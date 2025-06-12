#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->gBox_buttons->setTitle("Отображение прогресса");
    ui->rb_percentFormat->setText("В процентах");
    ui->rb_valueFormat->setText("Абсолютное значение");
    ui->pb_incrProgress->setText("Увеличить прогресс");

    ui->rb_percentFormat->setChecked(true);

    ui->prBar_progress->setMinimum(0);
    ui->prBar_progress->setMaximum(120);
    ui->prBar_progress->setValue(0);

    ui->pb_incrProgress->setCheckable(true);

    for(int i=1; i<=10; ++i){
        ui->cBox_elements->addItem(QString("Элемент №%1").arg(i));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switch_progress_format(){
    if(ui->rb_percentFormat->isChecked()){
        ui->prBar_progress->setFormat("%p%");
    }
    else{
        ui->prBar_progress->setFormat("%v");
    }
}

void MainWindow::on_pb_incrProgress_clicked()
{
    int value=ui->prBar_progress->value();
    if(value>=ui->prBar_progress->maximum()){
        value=0;
    }
    else{
        value+=10*ui->prBar_progress->maximum()/100;
    }
    ui->prBar_progress->setValue(value);
}


void MainWindow::on_rb_percentFormat_clicked()
{
    switch_progress_format();
}


void MainWindow::on_rb_valueFormat_clicked()
{
    switch_progress_format();
}

