REMOTE := raspberrypi.local

SSD1306.o:
	g++ -std=c++11 -c SSD1306.cpp

SSD1306.a: SSD1306.o
	ar rcs $@ $^

ssdtest: SSD1306.a
	g++ -o $@ -std=c++11 -lwiringPi \
	ssdtest.cpp \
	wPiGPIO.cpp \
	wPiSPI.cpp \
	Image.cpp \
	Ditherer.cpp \
	$^

clean:
	rm -f *.o *.a

transfer:
	scp *.cpp pi@$(REMOTE):~/ssd1306/
	scp *.h pi@$(REMOTE):~/ssd1306/
	scp Makefile pi@$(REMOTE):~/ssd1306/