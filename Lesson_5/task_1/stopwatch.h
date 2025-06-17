#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>

class Stopwatch : public QObject
{
    Q_OBJECT

private:
    QTimer* m_timer;
    double m_time;
    double m_interval;
    double m_lapTime;
    int m_lapsCount;

public:
    explicit Stopwatch(QObject *parent = nullptr);
    void start();
    void stop();

signals:
    void sig_send_time(double);
    void sig_send_lap(int,double);

public slots:
    void slot_update_time();
    void slot_clear_time();
    void slot_update_lap();
};

#endif // STOPWATCH_H
