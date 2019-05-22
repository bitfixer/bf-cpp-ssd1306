/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "SSD1306.h"
#include "wiringPi.h"

// Constants
#define SSD1306_I2C_ADDRESS 0x3C    // 011110+SA0+RW - 0x3C or 0x3D
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR 0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D

// Scrolling constants
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

class DummySPI : public SPI
{
public:
    void write(uint8_t byte)
    {
        
    }
    
    void write(uint8_t* buffer, int size)
    {
        
    }
    
    void set_clock_hz(int hz)
    {
        
    }
};

class DummyGPIO : public GPIO
{
public:
    void setup(int pin, DIR direction)
    {
        
    }
    
    void set_low(int pin)
    {
        
    }
    
    void set_high(int pin)
    {
        
    }
};

class SSD1306Base : public SSD1306 {
    
public:
 
    // Base class for SSD1306-based OLED displays.  Implementors should subclass
    // and provide an implementation for the initialize function.
    
    int _width;
    int _height;
    int _pages;
    int _rst;
    int _dc;
    uint8_t* _buffer;
    GPIO* _gpio;
    SPI* _spi;
    uint8_t _vccstate;
    
    SSD1306Base(int width, int height, int rst, int dc, GPIO* gpio, SPI* spi)
    : _width(width)
    , _height(height)
    , _pages(height/8)
    , _rst(rst)
    , _dc(dc)
    , _gpio(gpio)
    , _spi(spi)
    {
        _buffer = new uint8_t[_width * _pages];
        clear();

        if (!_spi)
            _spi = new DummySPI();
        
        if (!_gpio)
            _gpio = new DummyGPIO();

        if (_rst != -1)
        {
            _gpio->setup(_rst, GPIO::OUT);
        }
        
        _gpio->setup(_dc, GPIO::OUT);
    }
                
    virtual void initialize() = 0;

    void command(uint8_t c)
    {
        _gpio->set_low(_dc);
        _spi->write(c);
    }
    
    void data(uint8_t c)
    {
        _gpio->set_high(_dc);
        _spi->write(c);
    }
    
    void begin(uint8_t vccstate=SSD1306_SWITCHCAPVCC)
    {
        // Initialize display.
        // Save vcc state.
        _vccstate = vccstate;
        // Reset and initialize display.
        reset();
        initialize();
        // Turn on the display.
        command(SSD1306_DISPLAYON);
    }

    void reset()
    {
        if (_rst == -1)
            return;

        // TODO: implement reset
    }
    
    void display()
    {
        // Write display buffer to physical display.
        command(SSD1306_COLUMNADDR);
        command(0);                 // Column start address. (0 = reset)
        command(_width-1);          // Column end address.
        command(SSD1306_PAGEADDR);
        command(0);                 // Page start address. (0 = reset)
        command(_pages-1);          // Page end address.
        // Write buffer data.
        if (_spi != NULL) {
            // Set DC high for data.
            _gpio->set_high(_dc);
            // Write buffer.
            _spi->write(_buffer, _width*_pages);
        }
    }
    
    void setpixel(int x, int y, uint8_t val)
    {
        int bit = y % 8;
        int page = y / 8;
        
        if (val == 0)
        {
            _buffer[_width*page + x] &= ~(1 << bit);
        }
        else
        {
            _buffer[_width*page + x] |= 1 << bit;
        }
    }
                        
    void clear()
    {
        // Clear contents of image buffer.
        memset(_buffer, 0, _width*_pages);
    }

    void fill()
    {
        // turn on all pixels
        memset(_buffer, 255, _width*_pages);
    }
    
    void set_contrast(uint8_t contrast)
    {
        // Sets the contrast of the display.
        // Contrast should be a value between 0 and 255.
        if (contrast < 0 || contrast > 255)
        {
            printf("Contrast must be a value from 0 to 255 (inclusive).\n");
            return;
        }
        command(SSD1306_SETCONTRAST);
        command(contrast);
    }

    void dim(bool dim)
    {
        // Adjusts contrast to dim the display if dim is True, otherwise sets the
        // contrast to normal brightness if dim is False.
        
        // Assume dim display.
        uint8_t contrast = 0;
        // Adjust contrast based on VCC if not dimming.
        if (!dim)
        {
            if (_vccstate == SSD1306_EXTERNALVCC)
            {
                contrast = 0x9F;
            }
            else
            {
                contrast = 0xCF;
            }
        }
    }
};

class SSD1306_128_64 : public SSD1306Base
{
public:
    
    SSD1306_128_64(int rst, int dc, GPIO* gpio, SPI* spi)
    : SSD1306Base(128, 64, rst, dc, gpio, spi)
    {
        
    }

    void initialize()
    {
        // 128x64 pixel specific initialization.
        command(SSD1306_DISPLAYOFF);                   // 0xAE
        command(SSD1306_SETDISPLAYCLOCKDIV);           // 0xD5
        command(0x80);                                 // the suggested ratio 0x80
        command(SSD1306_SETMULTIPLEX);                 // 0xA8
        command(0x3F);
        command(SSD1306_SETDISPLAYOFFSET);             // 0xD3
        command(0x0);                                  // no offset
        command(SSD1306_SETSTARTLINE | 0x0);           // line #0
        command(SSD1306_CHARGEPUMP);                   // 0x8D
        if (_vccstate == SSD1306_EXTERNALVCC)
            command(0x10);
        else
            command(0x14);
        command(SSD1306_MEMORYMODE);                   // 0x20
        command(0x00);                                 // 0x0 act like ks0108
        command(SSD1306_SEGREMAP | 0x1);
        command(SSD1306_COMSCANDEC);
        command(SSD1306_SETCOMPINS);                   // 0xDA
        command(0x12);
        command(SSD1306_SETCONTRAST);                  // 0x81
        if (_vccstate == SSD1306_EXTERNALVCC)
            command(0x9F);
        else
            command(0xCF);
        command(SSD1306_SETPRECHARGE);                 // 0xd9
        if (_vccstate == SSD1306_EXTERNALVCC)
            command(0x22);
        else
            command(0xF1);
        command(SSD1306_SETVCOMDETECT);                // 0xDB
        command(0x40);
        command(SSD1306_DISPLAYALLON_RESUME);          // 0xA4
        command(SSD1306_NORMALDISPLAY);                // 0xA6
    }
};

SSD1306* SSD1306::NewSSD1306_128_64(int rst, int dc, GPIO* gpio, SPI* spi)
{
    return new SSD1306_128_64(rst, dc, gpio, spi);
}

// TODO: implement 128_32 and 96_16