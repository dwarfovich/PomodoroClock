#ifndef PARAMETERSDIALOG_HPP
#define PARAMETERSDIALOG_HPP

#include "clock.hpp"

#include <QDialog>

QT_BEGIN_NAMESPACE
class QSpinBox;
QT_END_NAMESPACE

class ParametersDialog : public QDialog {
    Q_OBJECT
public:
    explicit ParametersDialog (QWidget* parent = nullptr);

    ClockParameters clockParameters () const;
    void            setClockParameters (const ClockParameters& parameters);

private:
    void createGui ();

private:
    QSpinBox* m_workTimeSpinbox;
    QSpinBox* m_shortBreakSpinbox;
    QSpinBox* m_longBreakSpinbox;
    QSpinBox* m_maxShortBreaksSpinbox;
};

#endif // PARAMETERSDIALOG_HPP
