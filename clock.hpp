#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <QObject>
#include <QTimer>

using Byte = unsigned char;

enum class ClockStage : Byte {
    Work,
    ShortBreak,
    LongBreak
};

struct ClockParameters {
    int workTime       = 25 * 60;
    int shortBreakTime = 5 * 60;
    int longBreakTime  = 15 * 60;
    int maxShortBreaks = 4;
};

class Clock : public QObject {
    Q_OBJECT
public:
    explicit Clock (QObject* parent = nullptr);
    int secondsRemains () const;

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
    void clockValueChanged ();
    void timeout ();
    void secondTimeout ();
    void stageCompeted ();

private:
    QTimer*    m_timer;
    int        m_secondsRemains;
    Byte       m_shortBreaks;
    ClockStage m_currentStage;
    ClockParameters m_parameters;
};

#endif // CLOCK_HPP
