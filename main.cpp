#include <pico_lcd_i2c.hpp>
#include <stdio.h>

int main()
{
    stdio_init_all();

    float flotante(20.06);
    int numero(20);

    pico_lcd_i2c lcd1(0x27, 16, 2);
    lcd1.init_port(i2c0, 16, 17);
    lcd1.init();
    lcd1.backlight_on();

    int contador(0);

    while(true) {
    	lcd1.printlc(0, 0, "Contador: ");
    	lcd1.printlc(0, 13, contador);
    	contador++;
    	sleep_ms(500);
    	if(contador > 99) {
    		contador = 0;
    	}
    	lcd1.clear();
    }

    return 0;
}
