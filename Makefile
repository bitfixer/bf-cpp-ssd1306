REMOTE := raspberrypi.local

all:
	g++ -o ssdtest -std=c++11 -lwiringPi \
	ssdtest.cpp \
	SSD1306.cpp \
	wPiGPIO.cpp \
	wPiSPI.cpp \
	Image.cpp \
	Ditherer.cpp

transfer:
	scp *.cpp pi@$(REMOTE):~/ssd1306/
	scp *.h pi@$(REMOTE):~/ssd1306/
	scp Makefile pi@$(REMOTE):~/ssd1306/