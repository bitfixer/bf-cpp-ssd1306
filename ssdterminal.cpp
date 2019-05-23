#include "ssdterminal.h"
#include "SSD1306.h"
#include "ssd-gfx.h"
#include "Org_01.h"

class SSDTerminalImpl : public SSDTerminal {
public:
    SSDTerminalImpl(int dc, GPIO* gpio, SPI* spi)
    : _dc(dc)
    , _gpio(gpio)
    , _spi(spi)
    , _numlines(4)
    , _ssd(NULL)
    , _gfx(NULL)
    {
        _ssd = SSD1306::NewSSD1306_128_64(1, _dc, _gpio, _spi);
        _gfx = new SSDGFX(128, 64, _ssd);
        _gfx->setFont(&Org_01);

        _ssd->begin();
        _ssd->clear();
    }

    ~SSDTerminalImpl() 
    {
        if (_ssd)
        {
            delete _ssd;
        }

        if (_gfx)
        {
            delete _gfx;
        }
    }

    void clear()
    {
        _ssd->clear();
        _gfx->setCursor(0, 20);
        _ssd->display();
    }

    void print(const char* string)
    {
        _gfx->print((char*)string);
        _ssd->display();
    }

private:
    int _dc;
    GPIO* _gpio;
    SPI* _spi;
    int _numlines;

    SSD1306* _ssd;
    SSDGFX* _gfx;
};


SSDTerminal* SSDTerminal::NewSSDTerminal(int dc, GPIO* gpio, SPI* spi)
{
    return new SSDTerminalImpl(dc, gpio, spi);
}