#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "SeederSimulation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnectDisconnect();
    void updateSimulation();

private:
    Ui::MainWindow *ui;
    SeederSimulation simulation;
    QTimer *timer;
};

#endif // MAINWINDOW_H