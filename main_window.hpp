#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "clock_parameters.hpp"

#include <QMainWindow>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
class QSystemTrayIcon;
class QMenu;
class QMenuBar;
class QAction;
QT_END_NAMESPACE

class Clock;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow (QWidget* parent = nullptr);
    ~MainWindow ();

public slots:
    void updateTime ();

protected:
    void closeEvent (QCloseEvent* event) override;

private slots:
    void startClock ();
    void stopClock ();
    void buttonChecked (bool checked);
    void secondTimeout ();
    void onStageCompleted ();
    void trayIconActivated (QSystemTrayIcon::ActivationReason reason);
    void openPropertiesDialog ();

private:
    ClockParameters loadParameters ();
    void            saveParameters (const ClockParameters& parameters);

private:
    Ui::MainWindow* m_ui;
    QMenuBar*       m_menuBar;
    QMenu*   m_mainMenu;
    QAction* m_actionShowProperties;
    QSystemTrayIcon* m_trayIcon;
    QAction* m_actionMinimize;
    QAction* m_actionRestore;
    QAction* m_actionQuit;
    QMenu*   m_trayIconMenu;

    Clock* m_clock;
};
#endif // MAINWINDOW_HPP
