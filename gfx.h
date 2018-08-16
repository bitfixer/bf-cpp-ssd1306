#ifndef __gfx_h__
#define __gfx_h__

class GFX {
public:
    virtual void setpixel(int x, int y, uint8_t val) = 0;
};

#endif
