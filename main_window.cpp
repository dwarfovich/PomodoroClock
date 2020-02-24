#include "main_window.hpp"
#include "ui_main_window.h"
#include "parameters_dialog.hpp"

#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QSettings>

#include <QDebug>
#define DEB qDebug()

MainWindow::MainWindow (QWidget* parent)
    : QMainWindow{parent}
    , m_ui{new Ui::MainWindow}
    , m_clock{new Clock { this }}
    , m_trayIcon{new QSystemTrayIcon { this }} {
    m_ui->setupUi(this);
    m_ui->workButton->setChecked(true);
    m_ui->timeWidget->setFocusPolicy(Qt::NoFocus);
    QFont font;
    font.setPixelSize(48);
    m_ui->timeWidget->setFont(font);

    m_actionQuit = new QAction { "Quit", this };
    connect(m_actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    m_actionShowProperties = new QAction { "Properties", this };
    connect(m_actionShowProperties, &QAction::triggered, this, &MainWindow::openPropertiesDialog);
    m_menuBar  = new QMenuBar { this };
    m_mainMenu = new QMenu { "Main", m_menuBar };
    m_mainMenu->addAction(m_actionShowProperties);
    m_mainMenu->addSeparator();
    m_mainMenu->addAction(m_actionQuit);
    m_menuBar->addMenu(m_mainMenu);
    setMenuBar(m_menuBar);

    connect(m_ui->startButton, &QPushButton::clicked, this, &MainWindow::startClock);
    connect(m_ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopClock);
    connect(m_ui->workButton, &QPushButton::toggled, this, &MainWindow::buttonChecked);
    connect(m_ui->shortBreakButton, &QPushButton::toggled, this, &MainWindow::buttonChecked);
    connect(m_ui->longBreakButton, &QPushButton::toggled, this, &MainWindow::buttonChecked);
    connect(m_clock, &Clock::secondTimeout, this, &MainWindow::secondTimeout);
    connect(m_clock, &Clock::stageCompeted, this, &MainWindow::onStageCompleted);

    auto parameters = loadParameters();
    m_clock->setParameters(parameters);
    updateTime();

// quitAction = new QAction(tr("&Quit"), this);
// connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

// minimizeAction = new QAction(tr("Mi&nimize"), this);
// connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

// maximizeAction = new QAction(tr("Ma&ximize"), this);
// connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

// restoreAction = new QAction(tr("&Restore"), this);
// connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    m_actionMinimize = new QAction("Minimize", this);
    connect(m_actionMinimize, &QAction::triggered, this, &MainWindow::hide);

    m_actionRestore = new QAction("Restore", this);
    connect(m_actionRestore, &QAction::triggered, this, &MainWindow::showNormal);

    m_trayIconMenu = new QMenu { this };
    m_trayIconMenu->addAction(m_actionMinimize);
    m_trayIconMenu->addAction(m_actionRestore);
    m_trayIconMenu->addAction(m_actionQuit);

    m_trayIcon->setIcon(QIcon(":/Tomato-512.png"));
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->show();
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);

// QObject::connect(qApp, &QApplication::aboutToQuit, &mw, &MainWindow::close);
    connect(qApp, &QApplication::aboutToQuit, this, [this]() {
        saveParameters(m_clock->parameters());
    });

    setWindowTitle("Pomodor clock");
}

MainWindow::~MainWindow () {
    delete m_ui;
}

void MainWindow::updateTime () {
    int seconds = m_clock->secondsRemains();
    m_ui->timeWidget->setTime(QTime(0, seconds / 60, seconds % 60));
}

void MainWindow::closeEvent (QCloseEvent* event) {

    if (m_trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
// saveParameters(m_clock->parameters());
// event->accept();
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
        m_clock->setNextStage(ClockStage::Work);
    } else if (sender()->objectName().compare("shortBreakButton") == 0) {
        m_ui->workButton->setChecked(false);
        m_ui->longBreakButton->setChecked(false);
        m_clock->setNextStage(ClockStage::ShortBreak);
    } else if (sender()->objectName().compare("longBreakButton") == 0) {
        m_ui->workButton->setChecked(false);
        m_ui->shortBreakButton->setChecked(false);
        m_clock->setNextStage(ClockStage::LongBreak);
    }
    updateTime();
}

void MainWindow::secondTimeout () {
    updateTime();
}

void MainWindow::onStageCompleted () {
// setWindowState(windowState() & (~Qt::WindowMinimized | Qt::WindowActive));
    showNormal();
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

void MainWindow::trayIconActivated (QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        if (isVisible()) {
            hide();
        } else {
            showNormal();
        }
    }
}

void MainWindow::openPropertiesDialog () {
    ParametersDialog* dlg = new ParametersDialog { this };
    dlg->setClockParameters(m_clock->parameters());
    auto result = dlg->exec();
    if (result == QDialog::Accepted) {
        m_clock->setParameters(dlg->clockParameters());
        updateTime();
    }
    delete dlg;
}

ClockParameters MainWindow::loadParameters () {
// QSettings       settings { "BooSoft", "PomodoroClock" };
    QSettings       settings;
    ClockParameters parameters;
    parameters.workTime       = settings.value("work_time", 25 * 60).toInt();
    parameters.shortBreakTime = settings.value("short_break_time", 5 * 60).toInt();
    parameters.longBreakTime  = settings.value("long_break_time", 15 * 60).toInt();
    parameters.maxShortBreaks = settings.value("max_short_breaks", 4).toInt();
    DEB << parameters.workTime;

    return parameters;
}

void MainWindow::saveParameters (const ClockParameters& parameters) {
// QSettings settings { "BooSoft", "PomodoroClock" };
    DEB << "Save parameters";
    QSettings settings;
    settings.setValue("work_time", parameters.workTime);
    settings.setValue("short_break_time", parameters.shortBreakTime);
    settings.setValue("long_break_time", parameters.longBreakTime);
    settings.setValue("max_short_breaks", parameters.maxShortBreaks);
}
