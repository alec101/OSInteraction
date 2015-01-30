CXX=g++
CXX_FLAGS=-std=c++11 -Wall -pedantic -fno-common -fno-rtti
ARCH=$$(uname -m)

all:
	mkdir -p lib/linux-$(ARCH)/
	$(CXX) $(CXX_FLAGS) -shared -fPIC  src/*.cpp src/util/*.cpp -Wl,-soname,libOSI.so -o lib/linux-$(ARCH)/libOSI.so
