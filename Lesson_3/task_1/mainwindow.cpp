#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    conDialog=new ConnectionDialog(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pb_openWindow_clicked()
{
    conDialog->show();
}

