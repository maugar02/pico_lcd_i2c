#include <pico_lcd_i2c.hpp>

pico_lcd_i2c::pico_lcd_i2c(uint i2c_addr, uint8 lcd_columns, uint8 lcd_lines):
_i2c_port{nullptr},
_addr{i2c_addr},
generic_impl::lcd_i2c_generic(lcd_columns, lcd_lines)
{

}

pico_lcd_i2c::~pico_lcd_i2c()
{

}

int pico_lcd_i2c::i2c_write_byte(uint8 value)
{
    return i2c_write_blocking(_i2c_port, _addr, &value, 1, false);
}

void pico_lcd_i2c::wait_ms(uint32_t ms) const
{
    sleep_ms(ms);
}

void pico_lcd_i2c::wait_us(uint64_t us) const
{
    sleep_us(us);
}

void pico_lcd_i2c::init_port(i2c_inst_t *i2c_port, uint sda_pin, uint scl_pin)
{
    i2c_init(i2c_port, 100 * 1000);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);

    _i2c_port = i2c_port;
}
