CC := clang++
ARCH :=$(shell $(CC) -dumpmachine)

all:
	$(CC) -std=c++11 simple.cpp -I../../include/ -L../../lib/$(ARCH)/ -lOSI -lGL -o simple

clean:
	rm -rf simple
