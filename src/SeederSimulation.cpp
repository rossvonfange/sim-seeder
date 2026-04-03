#include "SeederSimulation.h"

SeederSimulation::SeederSimulation()
{
    isoStack = std::make_unique<IsoStackWrapper>();
}

SeederSimulation::~SeederSimulation()
{
    disconnect();
}

bool SeederSimulation::connect(const std::string &interface)
{
    return isoStack->initialize(interface);
}

void SeederSimulation::disconnect()
{
    isoStack->terminate();
}

bool SeederSimulation::isConnected() const
{
    return isoStack->isInitialized();
}

void SeederSimulation::setWheelSpeed(double speed)
{
    wheelSpeed = speed;
}

void SeederSimulation::setMeterRatio(double ratio)
{
    meterRatio = ratio;
}

void SeederSimulation::setWorkSwitch(bool state)
{
    workSwitch = state;
}

double SeederSimulation::getMeterSpeed() const
{
    return wheelSpeed * meterRatio;
}

void SeederSimulation::update()
{
    isoStack->setGroundSpeed(wheelSpeed);
    isoStack->setMeterSpeed(getMeterSpeed());
    isoStack->setWorkSwitch(workSwitch);
    isoStack->update();
}