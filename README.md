# Seeder Simulator

A Qt-based ISOBUS Seeder Simulator for Linux, featuring AgIsoStack-plus-plus integration. Simulates implement work switch, ground speed, and meter speed, and actively broadcasts ISOBUS PGNs for use with vcan0 and candump. Designed for both Task Controller (TC) and raw CAN tool visibility.

## Features
- Qt6 GUI for Seeder simulation
- AgIsoStack-plus-plus ISOBUS stack integration
- Periodic broadcast of Implement Work Switch (PGN 65094) at 10Hz
- Task Controller (TC) support and DDI updates
- Headless and GUI operation (Xvfb/VNC ready)
- CMake build system
- VCAN (virtual CAN) support for development/testing

## Quick Start

### Prerequisites
- Linux (tested on Ubuntu 22.04+)
- Qt6 (Widgets, Core, Gui)
- CMake >= 3.16
- g++ >= 9
- vcan kernel module (for virtual CAN)
- [AgIsoStack-plus-plus](https://github.com/Open-Agriculture/AgIsoStack-plus-plus) (MIT License)

### Build
```sh
mkdir -p build
cd build
cmake ..
make
```

### Run
```sh
# Setup vcan0 if not already
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0

# Run the simulator
./seeder-simulator
```

### Debugging in VS Code
- See `.vscode/launch.json` and `.gdbinit` for headless and GUI debug setup.

## Licensing & Attribution
- This project is licensed under the MIT License (see LICENSE).
- Uses [AgIsoStack-plus-plus](https://github.com/Open-Agriculture/AgIsoStack-plus-plus) (MIT License).

## Documentation
- See `docs/` for API and usage documentation.
- Main entry: `src/SeederSimulation.cpp`, `src/IsoStackWrapper.cpp`, `src/MainWindow.cpp`

## Contributing
Pull requests and issues are welcome!

## Authors
- cycix (2026)

---

This project is not affiliated with Open-Agriculture or the AgIsoStack-plus-plus maintainers. For ISOBUS stack details, see their repository.
