# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
include(../lcd_display/CMakeLists.txt)
include(../moto_datalogger/CMakeLists.txt)

project(MainDataLogger C CXX ASM)

pico_sdk_init()

# Add executable. Default name is the project name, 
add_executable(MainDataLogger mainDatalogger.cpp)

pico_add_extra_outputs(MainDataLogger)
target_link_libraries(MainDataLogger pico_stdlib lcd_display moto_datalogger)
