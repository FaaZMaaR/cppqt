#include "stopwatch.h"

Stopwatch::Stopwatch(QObject *parent)
    : QObject{parent}, m_timer{new QTimer(this)}, m_time{0}, m_interval{0.1}, m_lapTime{0}, m_lapsCount{0}
{
    connect(m_timer,&QTimer::timeout,this,&Stopwatch::slot_update_time);
}

void Stopwatch::start(){
    m_timer->start(m_interval*1000);
}

void Stopwatch::stop(){
    m_timer->stop();
}

void Stopwatch::slot_update_time(){
    m_time+=m_interval;
    m_lapTime+=m_interval;
    emit sig_send_time(m_time);
}

void Stopwatch::slot_clear_time(){
    m_time=0;
    m_lapTime=0;
    m_lapsCount=0;
    emit sig_send_time(m_time);
}

void Stopwatch::slot_update_lap(){
    emit sig_send_lap(++m_lapsCount,m_lapTime);
    m_lapTime=0;
}
