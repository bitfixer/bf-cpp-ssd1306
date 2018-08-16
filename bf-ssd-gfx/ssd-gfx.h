#ifndef __ssd_gfx_h__
#define __ssd_gfx_h__

#include "gfx.h"
#include "Adafruit_GFX.h"

class SSDGFX
{
public:
	SSDGFX(int16_t w, int16_t h, GFX* gfx)
	: Adafruit_GFX(w, h)
	, _gfx(gfx)
	{

	}

	void drawPixel(int16_t x, int16_t y, uint16_t color)
    {
        _gfx->setpixel(x, y, 255);
    }

private:
	GFX* _gfx;
};

#endif
