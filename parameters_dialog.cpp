#include "parameters_dialog.hpp"

#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include <QDebug>
#define DEB qDebug()

ParametersDialog::ParametersDialog (QWidget* parent)
    : QDialog{parent} {
    createGui();
}

ClockParameters ParametersDialog::clockParameters () const {
    ClockParameters parameters;
    parameters.workTime       = m_workTimeSpinbox->value() * 60;
    parameters.shortBreakTime = m_shortBreakSpinbox->value() * 60;
    parameters.longBreakTime  = m_longBreakSpinbox->value() * 60;
    parameters.maxShortBreaks = m_maxShortBreaksSpinbox->value();
    return parameters;
}

void ParametersDialog::setClockParameters (const ClockParameters& parameters) {
    m_workTimeSpinbox->setValue(parameters.workTime / 60);
    m_shortBreakSpinbox->setValue(parameters.shortBreakTime / 60);
    m_longBreakSpinbox->setValue(parameters.longBreakTime / 60);
    DEB << "ParametersDialog::setClockParameters" << parameters.maxShortBreaks;
    m_maxShortBreaksSpinbox->setValue(parameters.maxShortBreaks);
}

void ParametersDialog::createGui () {
    auto* verticalLayout = new QVBoxLayout { this };
    // Work time
    auto* horizontalLayout = new QHBoxLayout {};
    auto* label = new QLabel { "Work time", };
    horizontalLayout->addWidget(label);
    m_workTimeSpinbox = new QSpinBox {};
    m_workTimeSpinbox->setRange(1, 60);
    horizontalLayout->addWidget(m_workTimeSpinbox);
    verticalLayout->addLayout(horizontalLayout);
    // Short break
    horizontalLayout = new QHBoxLayout {};
    label = new QLabel { "Short break", };
    horizontalLayout->addWidget(label);
    m_shortBreakSpinbox = new QSpinBox {};
    m_shortBreakSpinbox->setRange(1, 60);
    horizontalLayout->addWidget(m_shortBreakSpinbox);
    verticalLayout->addLayout(horizontalLayout);
    // Long break
    horizontalLayout = new QHBoxLayout {};
    label = new QLabel { "Long break", };
    horizontalLayout->addWidget(label);
    m_longBreakSpinbox = new QSpinBox {};
    m_longBreakSpinbox->setRange(1, 60);
    horizontalLayout->addWidget(m_longBreakSpinbox);
    verticalLayout->addLayout(horizontalLayout);
    // Short breaks count
    horizontalLayout = new QHBoxLayout {};
    label = new QLabel { "Short breaks count", };
    horizontalLayout->addWidget(label);
    m_maxShortBreaksSpinbox = new QSpinBox {};
    horizontalLayout->addWidget(m_maxShortBreaksSpinbox);
    verticalLayout->addLayout(horizontalLayout);
    // Buttons
    horizontalLayout = new QHBoxLayout {};
    auto cancelButton = new QPushButton { "Cancel" };
    connect(cancelButton, &QPushButton::clicked, this, &ParametersDialog::reject);
    horizontalLayout->addWidget(cancelButton);
    auto acceptButton = new QPushButton { "Accept" };
    connect(acceptButton, &QPushButton::clicked, this, &ParametersDialog::accept);
    horizontalLayout->addWidget(acceptButton);
    verticalLayout->addLayout(horizontalLayout);

    setLayout(verticalLayout);
}
