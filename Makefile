#TODO: Add options to cross-compile to other OSES and archs (MingW and so on)
#TODO: Look into ranlib
#TODO: Make subdirectories recursivly and automaticly

CC :=clang++

#get architecture, should work EVERYWHERE
ARCH :=$(shell $(CC) -dumpmachine)

# No rtti and no exceptions are required for highly stable and memory use restricted programs, pretty much embedded system and high-end games for mobile or consoles.
# No common increases peformance and its more compilant to ISO
CC_FLAGS         :=-std=c++11 -fno-rtti -fno-common -fno-exceptions
CC_FLAGS_RELEASE :=$(CC_FLAGS) -O3
CC_FLAGS_DEBUG   :=$(CC_FLAGS) -g -Wall -Wpedantic

CC_INCLUDE :=-Iinclude/

SRC      :=$(wildcard src/*.cpp)
SRC_UTIL :=$(wildcard src/util/*.cpp)

RELEASE_DYNAMIC_OBJ_FILES :=$(addprefix build/$(ARCH)/,$(notdir $(SRC:.cpp=.o)))
RELEASE_DYNAMIC_OBJ_FILES +=$(addprefix build/$(ARCH)/util/,$(notdir $(SRC_UTIL:.cpp=.o)))

DEBUG_DYNAMIC_OBJ_FILES   :=$(addprefix build/$(ARCH)/,$(notdir $(SRC:.cpp=-d.o)))
DEBUG_DYNAMIC_OBJ_FILES   +=$(addprefix build/$(ARCH)/util/,$(notdir $(SRC_UTIL:.cpp=-d.o)))

RELEASE_STATIC_OBJ_FILES  :=$(addprefix build/$(ARCH)/,$(notdir $(SRC:.cpp=-s.o)))
RELEASE_STATIC_OBJ_FILES  +=$(addprefix build/$(ARCH)/util/,$(notdir $(SRC_UTIL:.cpp=-s.o)))

LIBS :=-lX11 -lXrandr -lXinerama -lGL -lGLEW -lGLU

#List avaliable options in case there's no autocomplete
all:
	@echo 
	@echo - Options are: release-dynamic, release-debug, release-static and clean.
	@echo

#create required directories (targets starting with "_" are considered private in scope)
_directories:
	@echo 
	@echo - Detected architecture: $(ARCH)
	@mkdir -p build/$(ARCH)/util
	@mkdir -p lib/$(ARCH)
	@echo
	@echo - Compiling outdated files

#Compiles every single file for a dynamic release
# -fPIC for position-independent code
build/$(ARCH)/%.o: src/%.cpp
	$(CC) $(CC_FLAGS_RELEASE) $(CC_INCLUDE) -fPIC -c -o $@ $<

#Same thing but with warning flags (Lot of warnings currently)
build/$(ARCH)/%-d.o: src/%.cpp
	$(CC) $(CC_FLAGS_DEBUG) $(CC_INCLUDE) -fPIC -c -o $@ $<

#this one will be a static archive so we dont need fPIC
build/$(ARCH)/%-s.o: src/%.cpp
	$(CC) $(CC_FLAGS_RELEASE) $(CC_INCLUDE) -c -o $@ $<

#This one strips the lib from all symbols
release-dynamic: _directories $(RELEASE_DYNAMIC_OBJ_FILES)
	@echo
	@echo Linking:
	$(CC) -s -shared -Wl,-soname,libOSI.so -o lib/$(ARCH)/libOSI.so $(RELEASE_DYNAMIC_OBJ_FILES) $(LIBS)
	@echo
	@echo - lib/$(ARCH)/libOSI.so
	@echo

#This one links with all possible symbols
debug-dynamic: _directories $(DEBUG_DYNAMIC_OBJ_FILES)
	@echo
	@echo Linking:
	$(CC) -g3 -shared -Wl,-soname,libOSI-d.so -o lib/$(ARCH)/libOSI-d.so $(DEBUG_DYNAMIC_OBJ_FILES) $(LIBS)
	@echo
	@echo - lib/$(ARCH)/libOSI-d.so
	@echo

#this creates the static archive
release-static: _directories $(RELEASE_STATIC_OBJ_FILES)
	@echo
	@echo Archiving:
	ar rcs lib/$(ARCH)/libOSI.a $(RELEASE_STATIC_OBJ_FILES)
	@echo
	@echo - lib/$(ARCH)/libOSI.a
	@echo

#clean everything!
clean:
	rm -rf build
	rm -rf lib