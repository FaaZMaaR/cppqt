#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setWindowTitle("Films");

    ui->setupUi(this);
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_request->setEnabled(false);
    ui->pb_clear->setEnabled(false);

    dataDb = new DbData(this);
    dataBase = new DataBase(this);
    msg = new QMessageBox(this);

    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    connect(dataDb, &DbData::sig_sendData, this, [&](QVector<QString> receivData){
        dataForConnect = receivData;
    });

    connect(dataBase, &DataBase::sig_SendDataFromDB, this, &MainWindow::ScreenDataFromDB);

    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    connect(dataBase, &DataBase::sig_SendStatusRequest, this, &MainWindow::ReceiveStatusRequestToDB);

    dataForConnect[hostName] = "981757-ca08998.tmweb.ru";
    dataForConnect[dbName] = "netology_cpp";
    dataForConnect[login] = "netology_usr_cpp";
    dataForConnect[pass] = "CppNeto3";
    dataForConnect[port] = "5432";
}

MainWindow::~MainWindow()
{

    dataBase->DisconnectFromDataBase(DB_NAME);
    if(tbView){
        delete tbView;
    }

    delete ui;
}

/*!
 * @brief Слот отображает форму для ввода данных подключения к БД
 */
void MainWindow::on_act_addData_triggered( )
{

    dataDb->show( );
}

/*!
 * @brief Слот выполняет подключение к БД. И отображает ошибки.
 */

void MainWindow::on_act_connect_triggered( )
{

    if(ui->lb_statusConnect->text() == "Отключено"){

       ui->lb_statusConnect->setText("Подключение");
       ui->lb_statusConnect->setStyleSheet("color : black");

       auto connectToDb = [this]{ dataBase->ConnectToDataBase(dataForConnect); };
       QFuture<void> futConn = QtConcurrent::run(connectToDb);

    }
    else{

        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Отключено");
        ui->act_connect->setText("Подключиться");
        ui->lb_statusConnect->setStyleSheet("color:red");
        ui->pb_request->setEnabled(false);
    }
}

/*!
 * \brief Обработчик кнопки "Получить"
 */
void MainWindow::on_pb_request_clicked()
{
    QString req=request;
    switch (ui->cb_category->currentIndex( ) + 1) {
    case requestComedy:
        req+="WHERE c.name = 'Comedy'";
        break;
    case requestHorrors:
        req+="WHERE c.name = 'Horror'";
        break;
    }
    auto reqToDb = [this,req]{dataBase->RequestToDB(req);};
    QFuture<void> futRcvData = QtConcurrent::run(reqToDb);
    ui->pb_clear->setEnabled(true);

}

/*!
 * \brief Слот отображает значение в QTableWidget
 * \param widget
 * \param typeRequest
 */
void MainWindow::ScreenDataFromDB(QTableView *view)
{

    tbView = view;
    ui->TableWid->layout( )->addWidget(tbView);

}
/*!
 * \brief Метод изменяет стотояние формы в зависимости от статуса подключения к БД
 * \param status
 */
void MainWindow::ReceiveStatusConnectionToDB(bool status)
{

    if(status){
        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_request->setEnabled(true);
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError( ).text( ));
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec( );
    }

}

/*!
 * \brief Метод обрабатывает ответ БД на поступивший запрос
 * \param err
 */
void MainWindow::ReceiveStatusRequestToDB(QSqlError err)
{

    if(err.isValid( )){
        msg->setText(err.text());
        msg->show( );
    }
    else{
        dataBase->ReadAnswerFromDB(ui->cb_category->currentIndex( ) + 1);
    }

}

void MainWindow::on_pb_clear_clicked()
{
    tbView->model()->removeColumns(0,tbView->model()->columnCount());
    ui->pb_clear->setEnabled(false);
}

