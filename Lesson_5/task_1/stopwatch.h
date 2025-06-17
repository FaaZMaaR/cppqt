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
    void clear();
    void update_lap();
    std::pair<int,double> get_lap();

signals:
    void sig_send_time(double);

public slots:
    void slot_update_time();
};

#endif // STOPWATCH_H
