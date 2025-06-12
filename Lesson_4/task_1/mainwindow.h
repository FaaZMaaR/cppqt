#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_incrProgress_clicked();

    void on_rb_percentFormat_clicked();

    void on_rb_valueFormat_clicked();

private:
    Ui::MainWindow *ui;

    void switch_progress_format();
};
#endif // MAINWINDOW_H
