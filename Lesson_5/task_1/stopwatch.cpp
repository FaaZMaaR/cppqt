#include "stopwatch.h"

Stopwatch::Stopwatch(QObject *parent)
    : QObject{parent}, m_timer{new QTimer(this)}, m_time{0}, m_interval{0.1}, m_lapTime{0}, m_lapsCount{1}
{
    connect(m_timer,&QTimer::timeout,this,&Stopwatch::slot_update_time);
}

void Stopwatch::start(){
    m_timer->start(m_interval*1000);
}

void Stopwatch::stop(){
    m_timer->stop();
}

void Stopwatch::clear(){
    m_time=0;
    m_lapTime=0;
    m_lapsCount=1;
}

void Stopwatch::update_lap(){
    ++m_lapsCount;
    m_lapTime=0;
}

std::pair<int,double> Stopwatch::get_lap(){
    return std::pair<int,double>(m_lapsCount,m_lapTime);
}

void Stopwatch::slot_update_time(){
    m_time+=m_interval;
    m_lapTime+=m_interval;
    emit sig_send_time(m_time);
}
