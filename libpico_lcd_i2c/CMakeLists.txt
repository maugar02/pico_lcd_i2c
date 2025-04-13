add_library(pico_lcd_i2c STATIC)

set_property(TARGET pico_lcd_i2c PROPERTY CXX_STANDARD 17)

target_include_directories(pico_lcd_i2c PRIVATE include)
target_include_directories(pico_lcd_i2c INTERFACE include)

target_sources(pico_lcd_i2c PRIVATE lcd_i2c_generic.cpp pico_lcd_i2c.cpp)

target_link_libraries(pico_lcd_i2c pico_stdlib hardware_i2c)