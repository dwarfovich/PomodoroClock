#include "clock.hpp"

#include <QTimer>

Clock::Clock (QObject* parent)
    : QObject{parent}
    , m_timer{new QTimer(this)}
    , m_secondsRemains{m_parameters.workTime}
    , m_shortBreaks{0}
    , m_currentStage{ClockStage::Work}
    , m_parameters{} {
    connect(m_timer, &QTimer::timeout, this, &Clock::onSecondTimeout);
}

int Clock::secondsRemains () const {
    return m_secondsRemains;
}

void Clock::start (ClockStage stage) {
    switch (stage) {
    case ClockStage::Work:
        m_secondsRemains = m_parameters.workTime;
        break;
    case ClockStage::ShortBreak:
        m_secondsRemains = m_parameters.shortBreakTime;
        break;
    case ClockStage::LongBreak:
        m_secondsRemains = m_parameters.longBreakTime;
        break;
    default:
        return;
    }
    m_currentStage = stage;
    m_timer->start(1000);
}

void Clock::stop () {
    m_timer->stop();
}

void Clock::onSecondTimeout () {
    --m_secondsRemains;
    if (m_secondsRemains == 0) {
        m_timer->stop();
        onStageCompeted();
    }
    emit secondTimeout();
}

void Clock::onStageCompeted () {
    if (m_currentStage == ClockStage::Work) {
        if (m_shortBreaks >= parameters().maxShortBreaks) {
            m_currentStage = ClockStage::LongBreak;
            m_shortBreaks  = 0;
        } else {
            m_currentStage = ClockStage::ShortBreak;
            ++m_shortBreaks;
        }
    } else {
        m_currentStage = ClockStage::Work;
    }

    emit stageCompeted();
}

ClockParameters Clock::parameters () const {
    return m_parameters;
}

void Clock::setParameters (const ClockParameters& parameters) {
    m_parameters     = parameters;
    m_secondsRemains = m_parameters.workTime;
}

ClockStage Clock::nextStage () const {
    return m_currentStage;
}

void Clock::setNextStage (ClockStage stage) {
    m_timer->stop();
    if (stage == ClockStage::Work) {
        m_secondsRemains = parameters().workTime;
    } else if (stage == ClockStage::ShortBreak) {
        m_secondsRemains = parameters().shortBreakTime;
    } else if (stage == ClockStage::LongBreak) {
        m_secondsRemains = parameters().longBreakTime;
    }
}
