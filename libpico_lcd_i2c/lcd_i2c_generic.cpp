#include <lcd_i2c_generic.hpp>
#include <cstring>

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

    // Convierte un entero a cadena de caracteres
    char *int_to_chars_10(char *str, int value, int &index)
	{
		// Vemos primero si no es cero
        // En caso de que sea cero, escribimos cero y retornamos
		if(value == 0) {
			str[index++] = '0';
			str[index++] = '\000';
			return str;
		}

		char nums[] = "0123456789";
		char temp[16] = "000000000000000";
		
		int i = index;
		int c(14);

		// Si es negativo agregamos el signo
		if(value < 0) {
			str[0] = '-';
			i++;
			value = -value;
			
		}
		
        // Agregamos los digitos a temp de forma descendente
        // Luego los agregamos a la cadena de salida
		while(value > 0) {
			int r = value % 10;
			temp[c] = nums[r];
			value /= 10;
			c--;	
		}
		// Incrementamos para volver al ultimo digito ingresado
		c++;
		// Copiamos lo que estaba en temp a la cadena de salida
		while(c < 16) {
			str[i] = temp[c];
			c++; i++;
		}
		index = i;
		return str;
	}

	// Convierte un entero sin signo a cadena de caracteres
	char *uint_to_chars_10(char *str, unsigned int value)
	{
        // Se hace lo mismo que en la funcion anterio
        // preo sin evaluar el signo.

		if(value == 0) {
			str[0] = '0';
			str[1] = '\000';
			return str;
		}

		char nums[] = "0123456789";
		char temp[16] = "000000000000000";
		
		int i(0);
		int c(14);
		
		while(value > 0) {
			unsigned int r = value % 10;
			temp[c] = nums[r];
			value /= 10;
			c--;	
		}
		
		c++;
		
		while(c < 16) {
			str[i] = temp[c];
			c++; i++;
		}
		
		return str;
	}

    // Convierte un numero en coma flotante a cadena de caracteres.
	char *float_to_chars_10(char *str, float value, uint pre=3U)
	{
		int real(0);          // Guarda la parte real
		float decimal(0.0f);  // Guarda la parte decimal
		int i(0);             // Indice para escribir en la cadena de salida
		int pre_mult(1);      // Multiplicador de la parte decimal

		real = static_cast<int>(value);   // Convertimos a entero.
		decimal = (value - real);         // Obtenemos la parte decimal

        // La precisión está limitada a valores entre 0 y 3
		if(pre > 3U)
			pre = 3U;
        if(pre == 0)
            pre = 1U;

        // Calculamos el multiplicador de presicion multiplicando tantas veces por
        // 10 como lo requiera el presición.
		for(int j(0); j < pre; j++) {
			pre_mult *= 10;
		}

        // Multiplicamos la parte decimal por el multiplicador y agregamos 0.5 
        // para mejorar la presición.
		decimal *= pre_mult;
        decimal += 0.5f;

        /* Si por alguna razon el valor decimal es igual o mayor que el multiplicador
        colocamos el valor decimal a cero e incrementamos el valor real. */
        if(static_cast<int>(decimal) >= pre_mult) {
            decimal = 0.0f;
            real++;
        }

        // Convertimos la parte real y la agregamos a la cadena de salida
        int_to_chars_10(str, real, i);
        // Eliminamos el caracter del fin de linea que le agrega la función anterior
        str[--i] = '.'; i++;
        // Si el decimal es cero agregamos los ceros de la presicion
        if(static_cast<int>(decimal) == 0) {

            for(int j(0); j < pre; j++) {
                str[i++] = '0';
            }

            str[i] = '\000';
        }

        // Si no continuamos y convertimos la parte decimal y la agregamos la cadena
		else int_to_chars_10(str, static_cast<int>(decimal), i);
		
        // Retornamos, la funcion anterior ya agrega el caracter de fin de linea
		return str;
	}
    
    lcd_i2c_generic::lcd_i2c_generic(uint8 columns, uint8 lines):
    _col{columns},
    _lin{lines},
    _backlight_bit{0x00},
    _fs_flags{LCD_FS_DEFAULT_FLAGS},
    _dc_flags{LCD_DC_DEFAULT_FLAGS},
    _ems_flags{LCD_EMS_DEFAULT_FLAGS},
    _sclrs_flag{false},
    _clr_next{false},
    _prev_buffer{nullptr},
    _new_buffer{nullptr}
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

    void lcd_i2c_generic::__printc()
    {
    	int l(-1);

    	for(uint i(0); i < _lin*_col; i++) {

    		if(i % _col == 0) l++;

    		if(_prev_buffer[i] != _new_buffer[i]) {

    			set_cursor(l, i % _col, true);
    			send_char(_new_buffer[i]);
    		}

    		_prev_buffer[i] = _new_buffer[i];
    	}
    }

    void lcd_i2c_generic::update()
    {
    	if(_sclrs_flag) __printc();
    }

    void lcd_i2c_generic::clear()
    {
    	if(_sclrs_flag) {
    		_clr_next = true;
    		return;
    	}

        send_command(LCD_CLEARDISPLAY, 0);
        wait_ms(2);
    }

    void lcd_i2c_generic::smooth_clear_on()
    {
    	int len = (_lin*_col) + 1;

    	// Reservamos memoria
    	if(_prev_buffer == nullptr) {

    		_prev_buffer = new char[len];
    	}

    	if(_new_buffer == nullptr) {
    		_new_buffer = new char[len];
    	}

    	for(int i(0); i < (len - 1); i++) {
    		_new_buffer[i] = _prev_buffer[i] = ' ';
    	}

    	_cursor = 0;

    	_sclrs_flag = true;
    }

    void lcd_i2c_generic::smooth_clear_off()
    {
    	if(_prev_buffer != nullptr) {
    		delete _prev_buffer;
    	}

    	if(_new_buffer != nullptr) {
    		delete _new_buffer;
    	}

    	_sclrs_flag = false;
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
    	if(_sclrs_flag) {

    		if(_clr_next) {

    			for(int i(0); i < (_lin * _col); i++){
    				if(_cursor == i) {
    					_new_buffer[i] = c;
    					continue;
    				}

    				_new_buffer[i] = ' ';
    			}
    			_clr_next = false;
    		}
    		_cursor++;
    		return;
    	}

        send_char(c);
    }

    void lcd_i2c_generic::print(const char *s)
    {
    	auto len = std::strlen(s);
    	auto last = len + _cursor;
    	int u=0;
    	if(_sclrs_flag) {
    		if(_clr_next) {

    			for(int i(0); i < (_lin * _col); i++){

    				if(i >= _cursor &&  i < last) {
    					_new_buffer[i] = s[u++];
    					continue;
    				}

    				_new_buffer[i] = ' ';
    			}
    			_clr_next = false;
    		}
    		else
    		{
    			for(int i(_cursor); i < (_lin * _col) && i < last; i++){

    				_new_buffer[i] = s[u++];
    			}
    		}

    		return;
    	}

        while(*s) {
            send_char(*s++);
        }
    }

    void lcd_i2c_generic::set_cursor(uint line, uint column, bool no_mode)
    {
    	if(_sclrs_flag && no_mode == false) {
    		_cursor = line*_col + column;
    		return;
    	}

        int line_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
        if ( line > _lin ) {
            line = _lin-1;
        }
        send_command(LCD_SETDDRAMADDR, (column + line_offsets[line]));
    }

    void lcd_i2c_generic::set_float_precision(uint p)
    {
    	_float_pre = p;
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
        char cad[12];
        int i(0);
        int_to_chars_10(cad, value, i);
        print(cad);
    }

    void lcd_i2c_generic::printlc(uint line, uint column, int value)
    {
        set_cursor(line, column);
        print(value);
    }

    void lcd_i2c_generic::print(uint value)
    {
        char cad[12];
        uint_to_chars_10(cad, value);
        print(cad);
    }

    void lcd_i2c_generic::printlc(uint line, uint column, uint value)
    {
        set_cursor(line, column);
        print(value);
    }

    void lcd_i2c_generic::print(float value)
    {
       	char cad[15];
       	float_to_chars_10(cad, value, _float_pre);
        print(cad);
    }


    void lcd_i2c_generic::printlc(uint line, uint column, float value)
    {
        set_cursor(line, column);
        print(value);
    }
}