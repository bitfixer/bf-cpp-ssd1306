REMOTE := raspberrypi.local
INCLUDE := -I. -I./bf-shared/bf-rpi-io -I./bf-shared -I./bf-ssd-gfx -I./Adafruit-GFX-Library -I./Adafruit-GFX-Library/Fonts
SOURCES := ssdtest.cpp bf-shared/Image.cpp bf-shared/Ditherer.cpp

%.o : %.cpp
	g++ -std=c++11 $(INCLUDE) -c $^

SSD1306.a: SSD1306.o
	ar rcs $@ $^

ssdtest: $(SOURCES) SSD1306.a
	make -C bf-shared/bf-rpi-io bf-rpi-io.a
	make -C bf-ssd-gfx bf-ssd-gfx.a
	g++ -o $@ -std=c++11 -DARDUINO=100 -DPROGMEM= $(INCLUDE) -lwiringPi \
	$^ \
	bf-shared/bf-rpi-io/bf-rpi-io.a \
	bf-ssd-gfx/bf-ssd-gfx.a

clean:
	rm -f *.o *.a

transfer:
	scp *.cpp pi@$(REMOTE):~/bf-cpp-ssd1306/
	scp *.h pi@$(REMOTE):~/bf-cpp-ssd1306/
	scp Makefile pi@$(REMOTE):~/bf-cpp-ssd1306/