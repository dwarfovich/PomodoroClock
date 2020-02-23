#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "clock.hpp"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow (QWidget* parent = nullptr);
    ~MainWindow ();

public slots:
    void updateTime ();

private slots:
    void startClock ();
    void stopClock ();
    void buttonChecked (bool checked);
    void secondTimeout ();
    void onStageCompleted ();

private:
    Ui::MainWindow* m_ui;
    Clock* m_clock;
};
#endif // MAINWINDOW_HPP
