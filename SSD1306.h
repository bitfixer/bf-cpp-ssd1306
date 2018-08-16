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

#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "bf-gpio.h"
#include "bf-spi.h"

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

class SSD1306 {
public:
    virtual ~SSD1306() {}
    
    virtual void initialize() = 0;
    virtual void command(uint8_t c) = 0;
    virtual void data(uint8_t c) = 0;
    virtual void begin(uint8_t vccstate=SSD1306_SWITCHCAPVCC) = 0;
    virtual void reset() = 0;
    virtual void setpixel(int x, int y, uint8_t val) = 0;

    // load bitmap into memory, 1 pixel per byte
    virtual void loadbitmap(uint8_t* bitmap) {};

    // load image buffer directly into ssd buffer memory. 8 pixels per byte packed
    virtual void loadimagebuffer(uint8_t* buffer) {};
    virtual void display() = 0;
    virtual void clear() = 0;
    virtual void fill() = 0;
    virtual void set_contrast(uint8_t contrast) = 0;
    virtual void dim(bool dim) = 0;
    
    static SSD1306* NewSSD1306_128_64(int rst, int dc, GPIO* gpio, SPI* spi);
};

#endif
