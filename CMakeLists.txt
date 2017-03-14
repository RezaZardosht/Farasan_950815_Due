cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME Farasan_950815_Due)
project(${PROJECT_NAME})

set(${CMAKE_PROJECT_NAME}_SKETCH Farasan_950815_Due.ino)
generate_arduino_firmware(${CMAKE_PROJECT_NAME})
set(${CMAKE_PROJECT_NAME}_BOARD DUE)
set(${CMAKE_PROJECT_NAME}_PORT COM3)
LINK_DIRECTORIES(C:/Users/Zardosht/Documents/Arduino/libraries)
include_directories(C:/Users/Zardosht/Documents/Arduino/libraries)
include_directories(C:/Program Files (x86)/Arduino/hardware\teensy/avr/cores/teensy3)

include_directories(Main/DS1307)
