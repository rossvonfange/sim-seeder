#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSimulation);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::onConnectDisconnect);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onConnectDisconnect()
{
    if (simulation.isConnected()) {
        simulation.disconnect();
        ui->connectButton->setText("Connect");
        timer->stop();
    } else {
        QString interface = ui->interfaceEdit->text();
        if (simulation.connect(interface.toStdString())) {
            ui->connectButton->setText("Disconnect");
            timer->start(100); // 10 Hz
        }
    }
}

void MainWindow::updateSimulation()
{
    simulation.setWheelSpeed(ui->wheelSpeedSpin->value());
    simulation.setMeterRatio(ui->meterRatioSpin->value());
    simulation.setWorkSwitch(ui->workSwitchCheck->isChecked());
    simulation.update();
    ui->meterSpeedLabel->setText(QString::number(simulation.getMeterSpeed()));
}