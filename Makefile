REMOTE := raspberrypi.local
INCLUDE := -I./bf-shared/bf-rpi-io -I./bf-shared

%.o : %.cpp
	g++ -std=c++11 $(INCLUDE) -c SSD1306.cpp

SSD1306.a: SSD1306.o
	ar rcs $@ $^

ssdtest: SSD1306.a
	g++ -o $@ -std=c++11 $(INCLUDE) -lwiringPi \
	ssdtest.cpp \
	bf-shared/Image.cpp \
	bf-shared/Ditherer.cpp \
	bf-shared/bf-rpi-io/bf-rpi-io.a \
	$^

clean:
	rm -f *.o *.a

transfer:
	scp *.cpp pi@$(REMOTE):~/bf-cpp-ssd1306/
	scp *.h pi@$(REMOTE):~/bf-cpp-ssd1306/
	scp Makefile pi@$(REMOTE):~/bf-cpp-ssd1306/