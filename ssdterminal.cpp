#include <stdio.h>
#include <string.h>
#include <math.h>
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
    , _cursorPhase(-1)
    , _cursorBlinkRate(2.0f)
    {
        _lines = new char*[_numlines];
        for (int i = 0; i < _numlines; i++)
        {
            _lines[i] = new char[_cols];
        }

        _cursorLine = new char[_cols];

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

        if (_cursorLine)
        {
            delete _cursorLine;
        }
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

        sprintf(_lines[_currline], "%s", string);
        _currline++;
        _ssd->clear();
        _gfx->setCursor(0,6);
        for (int i = 0; i < _currline; i++)
        {
            _gfx->print(_lines[i]);
            _gfx->print((char*)"\n");
        }

        _ssd->display();
    }

    int tick(float seconds)
    {
        int cursorPeriods = floor(seconds * _cursorBlinkRate);
        int newPhase = cursorPeriods % 2;
        if (_cursorPhase != newPhase)
        {
            int line = _currline - 1;
            if (line < 0)
            {
                line = 0;
            }
            
            _cursorPhase = newPhase;
            _ssd->clear();
            _gfx->setCursor(0,6);
            for (int i = 0; i < _currline; i++)
            {
                _gfx->print(_lines[i]);
                if (i == line && _cursorPhase == 0)
                {
                    _gfx->print((char*)"_");
                }
                _gfx->print((char*)"\n");
            }
            _ssd->display();
        }
        return 3456;
    }

private:
    int _dc;
    GPIO* _gpio;
    SPI* _spi;
    int _numlines;
    int _cols;
    int _currline;
    char** _lines;
    int _cursorPhase;
    char* _cursorLine;
    float _cursorBlinkRate;

    SSD1306* _ssd;
    SSDGFX* _gfx;
};


SSDTerminal* SSDTerminal::NewSSDTerminal(int dc, GPIO* gpio, SPI* spi)
{
    return new SSDTerminalImpl(dc, gpio, spi);
}