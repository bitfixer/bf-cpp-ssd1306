//
//  ssdtest.cpp
//  ssd1306
//
//  Created by Michael Hill on 8/13/18.
//  Copyright © 2018 Michael Hill. All rights reserved.
//

#include "SSD1306.h"
#include "wPiGPIO.h"
#include "wPiSPI.h"
#include "wiringPi.h"
#include "Image.hpp"
#include "Ditherer.hpp"
#include "ssd-gfx.h"
#include "Org_01.h"

unsigned char bw_colors[] =
{
    0,      0,      0,
    255,    255,    255
};

int num_bw_colors = 2;

int main(int argc, char** argv)
{
    GPIO* gpio = WiringPiGPIO::NewWiringPiGPIO();
    SPI* spi = WiringPiSPI::NewWiringPiSPI();
    
    SSD1306* ssd = SSD1306::NewSSD1306_128_64(0, 4, gpio, spi);
    
    ssd->begin();
    ssd->clear();
    
    /*
    Image pic(fopen("max2.ppm", "rb"));
    Palette bwPalette(bw_colors, num_bw_colors);
    Ditherer* ditherer = Ditherer::createFloydSteinbergDitherer();
    Image* im = ditherer->createDitheredImageFromImageWithPalette(pic, bwPalette);
    for (int w = 0; w < 128; w++)
    {
        for (int h = 0; h < 64; h++)
        {
            Pixel* p = im->pixelAt(w, h);
            if (p->rgb[0] == 1.0)
            {
                ssd->setpixel(w, h, 255);
            }
            else
            {
                ssd->setpixel(w, h, 0);
            }
        }
    }
    */

    SSDGFX* gfx = new SSDGFX(128, 64, ssd);
    gfx->drawCircle(64, 40, 10, 65535);
    gfx->setFont(&Org_01);
    gfx->setCursor(20,20);
    gfx->print((char*)"hello!");
    
    ssd->display();
    
    /*
    for (int w = 0; w < 128; w++)
    {
        for (int h = 0; h < 64; h++)
        {
            ssd->clear();
            ssd->setpixel(w, h, 255);
            ssd->display();
            delay(10);
        }
    }
    */
    
    delete ssd;
}
