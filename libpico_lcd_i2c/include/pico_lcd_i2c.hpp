#ifndef _PICO_LCD_I2C_HPP_
#define _PICO_LCD_I2C_HPP_

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "lcd_i2c_generic.hpp"

using uint = unsigned int;

class pico_lcd_i2c final : public generic_impl::lcd_i2c_generic {

    private:

    i2c_inst_t * _i2c_port;
    uint _addr;
    virtual int i2c_write_byte(uint8 value);
    virtual void wait_ms(uint32_t ms) const;
    virtual void wait_us(uint64_t us) const;

    public:
    pico_lcd_i2c() = delete;
    pico_lcd_i2c(uint i2c_addr, uint8 lcd_columns, uint8 lcd_lines);
    void init_port(i2c_inst_t *i2c_port, uint sda_pin, uint scl_pin);

    virtual ~pico_lcd_i2c();

};

#endif