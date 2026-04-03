#ifndef SEEDERSIMULATION_H
#define SEEDERSIMULATION_H

#include <string>
#include <memory>
#include "IsoStackWrapper.h"

class SeederSimulation
{
public:
    SeederSimulation();
    ~SeederSimulation();

    bool connect(const std::string &interface);
    void disconnect();
    bool isConnected() const;

    void setWheelSpeed(double speed);
    void setMeterRatio(double ratio);
    void setWorkSwitch(bool state);

    double getMeterSpeed() const;

    void update();

private:
    std::unique_ptr<IsoStackWrapper> isoStack;
    double wheelSpeed = 0.0;
    double meterRatio = 1.0;
    bool workSwitch = false;
};

#endif // SEEDERSIMULATION_H