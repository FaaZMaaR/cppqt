#include <QCoreApplication>
#include <QTcpServer>
#include <QSqlQuery>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTcpServer server;
    QSqlQuery query;

    return a.exec();
}
