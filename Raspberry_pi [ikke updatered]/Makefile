CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LIBS = $(shell pkg-config --cflags --libs libgpiod) -lwiringPi

SRC = main.cpp button.cpp lcd.cpp Uart.cpp
OUT = app

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LIBS)

run: all
	./$(OUT)
clean:
	rm -f *.o app
