#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stopwatch.h"

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

private:
    Ui::MainWindow *ui;

private:
    Stopwatch* m_stopwatch;

public slots:
    void slot_start_stop();
    void slot_update_time(double);
    void slot_clear_stopwatch();
    void slot_update_timelaps();
};
#endif // MAINWINDOW_H
