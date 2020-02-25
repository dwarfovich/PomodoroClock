#ifndef CLOCK_HPP
#define CLOCK_HPP

#include "clock_parameters.hpp"

#include <QObject>

class QTimer;

enum class ClockStage : unsigned char {
    Work,
    ShortBreak,
    LongBreak
};

class Clock : public QObject {
    Q_OBJECT
public:
    explicit Clock (QObject* parent = nullptr);

    int             secondsRemains () const;
    ClockParameters parameters () const;
    void            setParameters (const ClockParameters& parameters);
    ClockStage      nextStage () const;
    void            setNextStage (ClockStage stage);

public slots:
    void start (ClockStage stage);
    void stop ();

private slots:
    void onSecondTimeout ();
    void onStageCompeted ();

signals:
    void secondTimeout ();
    void stageCompeted ();

private:
    QTimer*    m_timer;
    int        m_secondsRemains;
    int        m_shortBreaks;
    ClockStage m_currentStage;
    ClockParameters m_parameters;
};

#endif // CLOCK_HPP
