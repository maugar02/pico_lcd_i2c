#include <pico_lcd_i2c.hpp>
#include <stdio.h>

int main()
{
    stdio_init_all();

    pico_lcd_i2c lcd1(0x27, 16, 2);
    lcd1.init_port(i2c0, 16, 17);
    lcd1.init();
    lcd1.backlight_on();
    lcd1.smooth_clear_on();

    int cl(0);

    while(true) {
    	
        lcd1.printlc(0, 0, "Cadena: ");
        lcd1.printlc(0, 8, cl);
        lcd1.printlc(1, 0, "Cadena2: ");
        lcd1.printlc(1, 8, cl-100);
        lcd1.update();
        sleep_ms(500);
        cl++;
        lcd1.clear();
    }
    
    return 0;
}
