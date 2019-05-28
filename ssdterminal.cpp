#include <stdio.h>
#include <string.h>
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
    , _numlines(9)
    , _cols(40)
    , _currline(0)
    , _ssd(NULL)
    , _gfx(NULL)
    , _lines(NULL)
    {
        _lines = new char*[_numlines];
        for (int i = 0; i < _numlines; i++)
        {
            _lines[i] = new char[_cols];
        }

        _ssd = SSD1306::NewSSD1306_128_64(-1, _dc, _gpio, _spi);
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

        for (int i = 0; i < _numlines; i++)
        {
            delete _lines[i];
        }
        delete _lines;
    }

    void clear()
    {
        for (int i = 0; i < _numlines; i++)
        {
            memset(_lines[i], 0, _cols);
        }

        _currline = 0;
        _ssd->clear();
        _gfx->setCursor(0,6);
        _ssd->display();
    }

    void print(const char* string)
    {
        if (_currline == _numlines)
        {
            for (int i = 1; i < _numlines; i++)
            {
                strcpy(_lines[i-1], _lines[i]);
            }
            _currline--;
        }

        sprintf(_lines[_currline], "%s\n", string);
        _currline++;
        _ssd->clear();
        _gfx->setCursor(0,6);
        for (int i = 0; i < _currline; i++)
        {
            _gfx->print(_lines[i]);
        }

        _ssd->display();
    }

private:
    int _dc;
    GPIO* _gpio;
    SPI* _spi;
    int _numlines;
    int _cols;
    int _currline;
    char** _lines;

    SSD1306* _ssd;
    SSDGFX* _gfx;
};


SSDTerminal* SSDTerminal::NewSSDTerminal(int dc, GPIO* gpio, SPI* spi)
{
    return new SSDTerminalImpl(dc, gpio, spi);
}