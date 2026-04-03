cmake_minimum_required(VERSION 3.16)
project(seeder-simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(isobus REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

qt6_wrap_ui(UI_HEADERS src/MainWindow.ui)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(seeder-simulator
    src/main.cpp
    src/SeederSimulation.cpp
    src/MainWindow.cpp
    src/IsoStackWrapper.cpp
    ${UI_HEADERS}
)

target_link_libraries(seeder-simulator
    Qt6::Widgets
    isobus::Isobus
    isobus::HardwareIntegration
    isobus::Utility
    Threads::Threads
)