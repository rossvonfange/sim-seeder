#ifndef ISOSTACKWRAPPER_H
#define ISOSTACKWRAPPER_H

#include <string>
#include <memory>
#include <isobus/isobus/isobus_speed_distance_messages.hpp>
#include <isobus/isobus/isobus_task_controller_client.hpp>
#include <isobus/isobus/isobus_virtual_terminal_client.hpp>
#include <isobus/isobus/can_network_manager.hpp>

class IsoStackWrapper
{
public:
    IsoStackWrapper();
    ~IsoStackWrapper();

    bool initialize(const std::string &interface);
    void terminate();
    bool isInitialized() const;

    void setGroundSpeed(double speed);
    void setMeterSpeed(double speed);
    void setWorkSwitch(bool state);

    void update();

private:
    bool initialized = false;
    std::shared_ptr<isobus::InternalControlFunction> internalECU;
    std::shared_ptr<isobus::DeviceDescriptorObjectPool> ddop;
    std::unique_ptr<isobus::TaskControllerClient> tcClient;
    std::shared_ptr<isobus::VirtualTerminalClient> vtClient;

    double groundSpeed = 0.0;
    double meterSpeed = 0.0;
    bool workSwitch = false;

    // For periodic PGN 65094 broadcast
    std::chrono::steady_clock::time_point lastWorkStateBroadcast = std::chrono::steady_clock::now();
};

#endif // ISOSTACKWRAPPER_H