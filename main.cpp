#include <pico_lcd_i2c.hpp>

int main()
{
    pico_lcd_i2c lcd1(0x27, 16, 2);
    lcd1.init_port(i2c0, 16, 17);
    lcd1.init();
    lcd1.backlight_on();

    while(true) {
        lcd1.prints("Hello!");
        sleep_ms(2000);
        lcd1.clear();

        lcd1.prints("Hola!");
        sleep_ms(2000);
        lcd1.clear();

        lcd1.prints("World/Mundo :)");
        sleep_ms(2000);
        lcd1.clear();
    }
}
