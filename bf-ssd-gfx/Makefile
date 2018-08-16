REMOTE := raspberrypi.local
INCLUDE := -I. -I../Adafruit-GFX-Library -I../Adafruit-GFX-Library/Fonts

%.o : %.cpp
	g++ -DARDUINO=100 -DPROGMEM= -std=c++11 $(INCLUDE) -c $^

bf-ssd-gfx.a: ../Adafruit-GFX-Library/Adafruit_GFX.o
	ar rcs $@ $^

clean:
	rm -f *.o *.a

transfer:
	scp *.cpp pi@$(REMOTE):~/bf-cpp-ssd1306/bf-ssd-gfx/
	scp *.h pi@$(REMOTE):~/bf-cpp-ssd1306/bf-ssd-gfx/
	scp Makefile pi@$(REMOTE):~/bf-cpp-ssd1306/bf-ssd-gfx/