#include <lcd_i2c_generic.hpp>
#include <string>

using std::string;
using std::to_string;

namespace generic_impl
{
    uint8 flag_enable(uint8 &value, uint8 flag)
    {
        return static_cast<uint8>(value |= flag);
    }

    uint8 flag_disable(uint8 &value, uint8 flag)
    {
        return static_cast<uint8>(value &= ~flag);
    }

    lcd_i2c_generic::lcd_i2c_generic(uint8 columns, uint8 lines):
    _col{columns},
    _lin{lines},
    _backlight_bit{0x00},
    _fs_flags{LCD_FS_DEFAULT_FLAGS},
    _dc_flags{LCD_DC_DEFAULT_FLAGS},
    _ems_flags{LCD_EMS_DEFAULT_FLAGS}
    {
        if(lines > 1) {
            _fs_flags |= LCD_FS_2LINE;
        }
    }

    lcd_i2c_generic::~lcd_i2c_generic() {}
    
    int lcd_i2c_generic::i2c_write_byte(uint8 value)
    {
        return false;
    }

    void lcd_i2c_generic::wait_ms(uint32_t ms) const
    {

    }

    void lcd_i2c_generic::wait_us(uint64_t us) const
    {

    }

    void lcd_i2c_generic::send(uint8 value, bool mode)
    {
        uint8 high_nib = mode | (value & 0xF0) | _backlight_bit;
        uint8 low_nib = mode | ((value << 4) & 0xF0) | _backlight_bit;

        i2c_write_byte(high_nib);
        pulse_enable(high_nib);
        i2c_write_byte(low_nib);
        pulse_enable(low_nib);

    }

    void lcd_i2c_generic::pulse_enable(uint8 value)
    {
        i2c_write_byte(value | LCD_ENABLE);
        wait_us(1);
        i2c_write_byte(value & ~LCD_ENABLE);
        wait_us(50);
    }

    void lcd_i2c_generic::send_command(uint8 command, uint8 command_flags)
    {
        uint8 value = command | command_flags;
        send(value, false);
    }

    void lcd_i2c_generic::send_char(char c)
    {
        send(c, true);
    }

    void lcd_i2c_generic::init()
    {
        constexpr uint8 init_num(0x03 << 4);

        i2c_write_byte(_backlight_bit);
        wait_ms(1000);

        i2c_write_byte(init_num);
        pulse_enable(init_num);
        wait_ms(5);

        i2c_write_byte(init_num);
        pulse_enable(init_num);
        wait_ms(5);

        i2c_write_byte(init_num);
        pulse_enable(init_num);
        wait_us(150);

        i2c_write_byte(0x02 << 4);
        pulse_enable(0x02 << 4);

        send_command(LCD_FUNCTIONSET, _fs_flags);
        send_command(LCD_DISPLAYCONTROL, 0);
        clear();
        send_command(LCD_ENTRYMODESET, _ems_flags);
        return_home();
        send_command(LCD_DISPLAYCONTROL, _dc_flags);
    }

    void lcd_i2c_generic::clear()
    {
        send_command(LCD_CLEARDISPLAY, 0);
        wait_ms(2);
    }

    void lcd_i2c_generic::return_home()
    {
        send_command(LCD_RETURNHOME, 0);
        wait_ms(2);
    }

    void lcd_i2c_generic::backlight_on()
    {
        _backlight_bit = LCD_BACKLIGHT;
        i2c_write_byte(_backlight_bit);
    }

    void lcd_i2c_generic::backlight_off()
    {
        _backlight_bit = 0x00;
        i2c_write_byte(_backlight_bit);
    }

    void lcd_i2c_generic::cursor_on()
    {
        send_command(LCD_DISPLAYCONTROL, flag_enable(_dc_flags, LCD_DC_CURSOR));
    }

    void lcd_i2c_generic::cursor_off()
    {
        send_command(LCD_DISPLAYCONTROL, flag_disable(_dc_flags, LCD_DC_CURSOR));
    }

    void lcd_i2c_generic::blink_on()
    {
        send_command(LCD_DISPLAYCONTROL, flag_enable(_dc_flags, LCD_DC_BLINK));
    }

    void lcd_i2c_generic::blink_off()
    {
        send_command(LCD_DISPLAYCONTROL, flag_disable(_dc_flags, LCD_DC_BLINK));
    }

    void lcd_i2c_generic::print(char c)
    {
        send_char(c);
    }

    void lcd_i2c_generic::print(const char *s)
    {
        while(*s) {
            send_char(*s++);
        }
    }

    void lcd_i2c_generic::set_cursor(uint line, uint column)
    {
        int line_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
        if ( line > _lin ) {
            line = _lin-1;
        }
        send_command(LCD_SETDDRAMADDR, (column + line_offsets[line]));
    }

    void lcd_i2c_generic::printlc(uint line, uint column, char c)
    {
        set_cursor(line, column);
        print(c);
    }

    void lcd_i2c_generic::printlc(uint line, uint column, const char *s)
    {
        set_cursor(line, column);
        print(s);
    }

    void lcd_i2c_generic::print(int value)
    {
        string v = to_string(value);
        print(v.c_str());
    }

    void lcd_i2c_generic::printlc(uint line, uint column, int value)
    {
        set_cursor(line, column);
        print(value);
    }

    void lcd_i2c_generic::print(float value)
    {
        string v = to_string(value);
        print(v.c_str());
    }

    void lcd_i2c_generic::printlc(uint line, uint column, float value)
    {
        set_cursor(line, column);
        print(value);
    }
}