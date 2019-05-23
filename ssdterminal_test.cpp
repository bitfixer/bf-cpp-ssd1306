#include "wPiGPIO.h"
#include "wPiSPI.h"
#include "ssdterminal.h"
#include <stdlib.h>

int main(int argc, char** argv)
{
    GPIO* gpio = WiringPiGPIO::NewWiringPiGPIO();
    SPI* spi = WiringPiSPI::NewWiringPiSPI(1, gpio, 2);
    spi->set_clock_hz(atoi(argv[1]));

    SSDTerminal* term = SSDTerminal::NewSSDTerminal(4, gpio, spi);
    term->print("hello there.\n");
    term->print("what's up\n");
    return 0;
}