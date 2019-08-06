INCLUDE := -I. -I./bf-shared/bf-rpi-io -I./bf-shared -I./bf-ssd-gfx -I./Adafruit-GFX-Library -I./Adafruit-GFX-Library/Fonts
SOURCES := ssdtest.cpp bf-shared/Image.cpp bf-shared/Ditherer.cpp

dep:
	git submodule init
	git submodule update

%.o : %.cpp
	g++ -std=c++11 -DARDUINO=100 -DPROGMEM= $(INCLUDE) -c $^

SSD1306.a: SSD1306.o
	ar rcs $@ $^

bf-ssd-gfx/%.o:
	make -C bf-ssd-gfx bf-ssd-gfx.o

ssdterminal.a: ssdterminal.o SSD1306.o bf-ssd-gfx/Adafruit_GFX.o
	ar rcs $@ $^

ssdtest: $(SOURCES) SSD1306.a
	make -C bf-shared/bf-rpi-io bf-rpi-io.a
	make -C bf-ssd-gfx bf-ssd-gfx.a
	g++ -o $@ -std=c++11 -DARDUINO=100 -DPROGMEM= $(INCLUDE) -lwiringPi \
	$^ \
	bf-shared/bf-rpi-io/bf-rpi-io.a \
	bf-ssd-gfx/bf-ssd-gfx.a

ssdterminal_test: ssdterminal_test.cpp ssdterminal.a
	make -C bf-shared/bf-rpi-io bf-rpi-io.a
	g++ -o $@ -std=c++11 $(INCLUDE) -lwiringPi $^ bf-shared/bf-rpi-io/bf-rpi-io.a

clean:
	rm -f *.o *.a