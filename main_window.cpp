#include "main_window.hpp"
#include "ui_main_window.h"

#include <QDebug>
#define DEB qDebug()

MainWindow::MainWindow (QWidget* parent)
    : QMainWindow{parent}
    , m_ui{new Ui::MainWindow}
    , m_clock{new Clock { this }} {
    m_ui->setupUi(this);
    m_ui->workButton->setChecked(true);
    m_ui->timeWidget->setFocusPolicy(Qt::NoFocus);
    QFont font;
    font.setPixelSize(48);
    m_ui->timeWidget->setFont(font);

    connect(m_ui->startButton, &QPushButton::clicked, this, &MainWindow::startClock);
    connect(m_ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopClock);
    connect(m_ui->workButton, &QPushButton::toggled, this, &MainWindow::buttonChecked);
    connect(m_ui->shortBreakButton, &QPushButton::toggled, this, &MainWindow::buttonChecked);
    connect(m_ui->longBreakButton, &QPushButton::toggled, this, &MainWindow::buttonChecked);
    connect(m_clock, &Clock::secondTimeout, this, &MainWindow::secondTimeout);
    connect(m_clock, &Clock::stageCompeted, this, &MainWindow::onStageCompleted);

    ClockParameters p;
    p.workTime       = 5;
    p.shortBreakTime = 2;
    p.longBreakTime  = 2;
    p.maxShortBreaks = 1;
    m_clock->setParameters(p);
    updateTime();
}

MainWindow::~MainWindow () {
    delete m_ui;
}

void MainWindow::updateTime () {
    int seconds = m_clock->secondsRemains();
    m_ui->timeWidget->setTime(QTime(0, seconds / 60, seconds % 60));
}

void MainWindow::startClock () {
    ClockStage stage;
    if (m_ui->workButton->isChecked()) {
        stage = ClockStage::Work;
    } else if (m_ui->shortBreakButton->isChecked()) {
        stage = ClockStage::ShortBreak;
    } else {
        stage = ClockStage::LongBreak;
    }
    m_ui->startButton->setDisabled(true);
    m_ui->clockStageFrame->setDisabled(true);
    m_clock->start(stage);
    updateTime();
}

void MainWindow::stopClock () {
    m_clock->stop();
    m_ui->startButton->setEnabled(true);
    m_ui->clockStageFrame->setEnabled(true);
}

void MainWindow::buttonChecked (bool checked) {
    if (!checked) {
        return;
    }
    if (sender()->objectName().compare("workButton") == 0) {
        m_ui->shortBreakButton->setChecked(false);
        m_ui->longBreakButton->setChecked(false);
    } else if (sender()->objectName().compare("shortBreakButton") == 0) {
        m_ui->workButton->setChecked(false);
        m_ui->longBreakButton->setChecked(false);
    } else if (sender()->objectName().compare("longBreakButton") == 0) {
        m_ui->workButton->setChecked(false);
        m_ui->shortBreakButton->setChecked(false);
    }
}

void MainWindow::secondTimeout () {
    updateTime();
}

void MainWindow::onStageCompleted () {
    setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
    stopClock();

    auto nextStage = m_clock->nextStage();
    if (nextStage == ClockStage::Work) {
        m_ui->workButton->setChecked(true);
    } else if (nextStage == ClockStage::ShortBreak) {
        m_ui->shortBreakButton->setChecked(true);
    } else {
        m_ui->longBreakButton->setChecked(true);
    }
}
