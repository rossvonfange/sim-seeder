#include "IsoStackWrapper.h"
#include <isobus/hardware_integration/socket_can_interface.hpp>
#include <isobus/hardware_integration/can_hardware_interface.hpp>
#include <isobus/utility/system_timing.hpp>
#include <isobus/isobus/isobus_standard_data_description_indices.hpp>
#include <thread>

IsoStackWrapper::IsoStackWrapper()
{
    // Members will be initialized in initialize
}

IsoStackWrapper::~IsoStackWrapper()
{
    terminate();
}

bool IsoStackWrapper::initialize(const std::string &interface)
{
    if (initialized) return true;

    auto canDriver = std::make_shared<isobus::SocketCANInterface>(interface);
    isobus::CANHardwareInterface::set_number_of_can_channels(1);
    isobus::CANHardwareInterface::assign_can_channel_frame_handler(0, canDriver);

    if (!isobus::CANHardwareInterface::start() || !canDriver->get_is_valid())
    {
        return false;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    isobus::NAME deviceNAME(0);
    deviceNAME.set_arbitrary_address_capable(true);
    deviceNAME.set_industry_group(2);
    deviceNAME.set_device_class(4);
    deviceNAME.set_function_code(static_cast<std::uint8_t>(isobus::NAME::Function::RateControl));
    deviceNAME.set_identity_number(2);
    deviceNAME.set_ecu_instance(0);
    deviceNAME.set_function_instance(0);
    deviceNAME.set_device_class_instance(0);
    deviceNAME.set_manufacturer_code(1407);

    internalECU = isobus::CANNetworkManager::CANNetwork.create_internal_control_function(deviceNAME, 0);

    // For TC
    const isobus::NAMEFilter filterTaskController(isobus::NAME::NAMEParameters::FunctionCode, static_cast<std::uint8_t>(isobus::NAME::Function::TaskController));
    const isobus::NAMEFilter filterTaskControllerInstance(isobus::NAME::NAMEParameters::FunctionInstance, 0);
    const isobus::NAMEFilter filterTaskControllerIndustryGroup(isobus::NAME::NAMEParameters::IndustryGroup, static_cast<std::uint8_t>(isobus::NAME::IndustryGroup::AgriculturalAndForestryEquipment));
    const isobus::NAMEFilter filterTaskControllerDeviceClass(isobus::NAME::NAMEParameters::DeviceClass, static_cast<std::uint8_t>(isobus::NAME::DeviceClass::NonSpecific));
    const std::vector<isobus::NAMEFilter> tcNameFilters = { filterTaskController, filterTaskControllerInstance, filterTaskControllerIndustryGroup, filterTaskControllerDeviceClass };
    auto partnerTC = isobus::CANNetworkManager::CANNetwork.create_partnered_control_function(0, tcNameFilters);

    tcClient = std::make_unique<isobus::TaskControllerClient>(partnerTC, internalECU, nullptr);

    // Create simple DDOP
    ddop = std::make_shared<isobus::DeviceDescriptorObjectPool>();
    std::array<std::uint8_t, 7> localizationData = { 'e', 'n', 0b01010000, 0x00, 0b01010101, 0b01010101, 0xFF };
    ddop->add_device("Seeder Simulator", "1.0.0", "123", "IS1.2", localizationData, std::vector<std::uint8_t>(), deviceNAME.get_full_name());

    // Add process data
    ddop->add_device_process_data("Work State", 415, isobus::NULL_OBJECT_ID, static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::OnChange), 1);
    ddop->add_device_process_data("Ground Speed", 161, isobus::NULL_OBJECT_ID, static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::TimeInterval), 2);
    ddop->add_device_process_data("Meter Speed", 162, isobus::NULL_OBJECT_ID, static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::PropertiesBit::MemberOfDefaultSet), static_cast<std::uint8_t>(isobus::task_controller_object::DeviceProcessDataObject::AvailableTriggerMethods::TimeInterval), 3);

    tcClient->configure(ddop, 1, 255, 255, true, true, true, false, true);
    tcClient->initialize(true);

    initialized = true;
    return true;
}

void IsoStackWrapper::terminate()
{
    if (initialized)
    {
        tcClient->terminate();
        isobus::CANHardwareInterface::stop();
        initialized = false;
    }
}

bool IsoStackWrapper::isInitialized() const
{
    return initialized;
}

void IsoStackWrapper::setGroundSpeed(double speed)
{
    groundSpeed = speed;
}

void IsoStackWrapper::setMeterSpeed(double speed)
{
    meterSpeed = speed;
}

void IsoStackWrapper::setWorkSwitch(bool state)
{
    workSwitch = state;
}

void IsoStackWrapper::update()
{
    if (!initialized) return;

    // Send ground based speed message (PGN 65098)
    std::uint16_t speed_mm_s = static_cast<std::uint16_t>(groundSpeed * 1000);
    std::vector<std::uint8_t> data(8, 0);
    data[0] = speed_mm_s & 0xFF;
    data[1] = (speed_mm_s >> 8) & 0xFF;
    data[2] = 0; // direction
    isobus::CANNetworkManager::CANNetwork.send_can_message(0xFEF2, data.data(), static_cast<std::uint32_t>(data.size()), internalECU);

    // --- Active periodic broadcast of Implement Work Switch (PGN 65094 / 0xFE46) ---
    // This is for raw visibility in candump and non-TC-aware tools (ISO 11783-7)
    constexpr uint32_t PGN_65094 = 0xFE46;
    constexpr std::chrono::milliseconds BROADCAST_INTERVAL(100); // 10Hz
    auto now = std::chrono::steady_clock::now();
    if (now - lastWorkStateBroadcast >= BROADCAST_INTERVAL && internalECU && internalECU->get_address_valid()) {
        std::array<uint8_t, 8> workPayload{};
        // Set all unused/reserved bits to 0xFF (Don't Care)
        workPayload.fill(0xFF);
        // Map workSwitch to Service/Work State bits (bits 0-1 of byte 0)
        // 00 = Out of Work, 01 = In Work
        workPayload[0] = (workSwitch ? 0x01 : 0x00) | (0xFF & 0xFC); // bits 2-7 = 1s (don't care)
        isobus::CANNetworkManager::CANNetwork.send_can_message(
            PGN_65094,
            workPayload.data(),
            static_cast<uint32_t>(workPayload.size()),
            internalECU,
            nullptr // global broadcast
        );
        lastWorkStateBroadcast = now;
    }

    // Update PD
    // tcClient->update_process_data_value(1, workSwitch ? 1 : 0);
    // tcClient->update_process_data_value(2, static_cast<std::int32_t>(groundSpeed * 1000));
    // tcClient->update_process_data_value(3, static_cast<std::int32_t>(meterSpeed * 1000));

    tcClient->update();
}