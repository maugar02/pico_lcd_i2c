#ifndef _LCD_I2C_GENERIC_
#define _LCD_I2C_GENERIC_

#include <stdint.h>

using uint8 = uint8_t;

namespace generic_impl
{
    // LCD conmmands
    constexpr uint8 LCD_CLEARDISPLAY(0x01);
    constexpr uint8 LCD_RETURNHOME(0x02);
    constexpr uint8 LCD_ENTRYMODESET(0x04);
    constexpr uint8 LCD_DISPLAYCONTROL(0x08);
    constexpr uint8 LCD_CURSORDISPLAYSHIFT(0x10);
    constexpr uint8 LCD_FUNCTIONSET(0x20);
    constexpr uint8 LCD_SETCGRAMADDR(0x40);
    constexpr uint8 LCD_SETDDRAMADDR(0x80);

    // Flags for entry mode set command
    constexpr uint8 LCD_EMS_RIGHT(0x00);
    constexpr uint8 LCD_EMS_LEFT(0x02);
    constexpr uint8 LCD_EMS_SHIFT(0x01);
    constexpr uint8 LCD_EMS_NOSHIFT(0x00);

    // Flags for display on/off control
    constexpr uint8 LCD_DC_ON(0x04);
    constexpr uint8 LCD_DC_OFF(0x00);
    constexpr uint8 LCD_DC_CURSOR(0x02);
    constexpr uint8 LCD_DC_NOCURSOR(0x00);
    constexpr uint8 LCD_DC_BLINK(0x01);
    constexpr uint8 LCD_DC_NOBLINK(0x00);

    // Flags for function set command
    constexpr uint8 LCD_FS_4BITMODE(0x00);
    constexpr uint8 LCD_FS_8BITMODE(0x10);
    constexpr uint8 LCD_FS_1LINE(0x00);
    constexpr uint8 LCD_FS_2LINE(0x08);
    constexpr uint8 LCD_FS_5x8DOTS(0x00);
    constexpr uint8 LCD_FS_5x10DOTS(0x04);

    // LCD backlight control
    constexpr uint8 LCD_BACKLIGHT(0x08);

    // LCD enable pin control
    constexpr uint8 LCD_ENABLE(0x04);

    constexpr uint8 LCD_EMS_DEFAULT_FLAGS(LCD_EMS_LEFT | LCD_EMS_NOSHIFT);
    constexpr uint8 LCD_FS_DEFAULT_FLAGS(LCD_FS_4BITMODE | LCD_FS_1LINE | LCD_FS_5x8DOTS);
    constexpr uint8 LCD_DC_DEFAULT_FLAGS(LCD_DC_ON | LCD_DC_NOBLINK | LCD_DC_NOCURSOR);

    class lcd_i2c_generic {

        protected:

        uint8 _col;
        uint8 _lin;
        uint8 _dc_flags;
        uint8 _ems_flags;
        uint8 _fs_flags;
        uint8 _backlight_bit;

        void send(uint8 value, bool mode);
        void pulse_enable(uint8 value);
        void send_command(uint8 command, uint8 command_flags);
        void send_char(char c);

        virtual int i2c_write_byte(uint8 value);
        virtual void wait_ms(uint32_t ms) const;
        virtual void wait_us(uint64_t us) const;

        public:

        lcd_i2c_generic() = delete;

        lcd_i2c_generic(uint8 columns, uint8 lines);

        void init();
        void clear();
        void return_home();

        void backlight_on();
        void backlight_off();
        void cursor_on();
        void cursor_off();
        void blink_on();
        void blink_off();
        
        void printc(char c);
        void prints(const char *s);

        virtual ~lcd_i2c_generic();
    };
}

#endif