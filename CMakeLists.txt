cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(lcd_test CXX C ASM)

set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(libpico_lcd_i2c)

add_executable(lcd_test main.cpp)

target_link_libraries(lcd_test pico_lcd_i2c)

pico_add_extra_outputs(lcd_test)
