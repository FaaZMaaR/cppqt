#include "database.h"
#include <cstdint>
#include <qcontainerfwd.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qtablewidget.h>

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase;
    tableView = new QTableView;
    queryModel = new QSqlQueryModel;

}

DataBase::~DataBase()
{
    delete dataBase;
    delete tableModel;
    delete queryModel;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{

    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);

}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{

    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());

    bool statusConnect = dataBase->open( );
    emit sig_SendStatusConnection(statusConnect);
}

/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{

    dataBase->database(nameDb);
    dataBase->close( );

    delete tableModel;
    tableModel=nullptr;

}

/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::RequestToDB(QString request)
{

    delete tableModel;
    tableModel=new QSqlTableModel(nullptr,*dataBase);
    tableModel->setTable("film");
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tableModel->select();
    tableModel->setHeaderData(1, Qt::Horizontal, tr("Название"));
    tableModel->setHeaderData(2, Qt::Horizontal, tr("Описание"));

    queryModel->setQuery(request,*dataBase);
    queryModel->setHeaderData(0, Qt::Horizontal, tr("Название"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("Описание"));

    emit sig_SendStatusRequest(queryModel->lastError());

}

/*!
 * \brief Метод читае ответ, полученный от БД при помощи QSqlQuery
 * \param answerType - тип ответа
 */
void DataBase::ReadAnswerFromDB(int resultType)
{

    switch(resultType){
    case requestAllFilms:
        tableView->setModel(tableModel);
        tableView->hideColumn(0);
        for(int i=3; i<tableModel->columnCount(); ++i){
            tableView->hideColumn(i);
        }
        break;
    case requestComedy:
    case requestHorrors:
        tableView->setModel(queryModel);
        break;
    }

    emit sig_SendDataFromDB(tableView);

}


/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError( );
}
