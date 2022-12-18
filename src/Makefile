.PHONY: all clean

CC = gcc
MAKE ?= make
RAYLIB_PATH=../../raylib
BINARY ?= ../bin/game
PLATFORM ?= PLATFORM_DESKTOP

CFLAGS = -std=c99 -Wall -Wno-missing-braces -Wunused-result -D_DEFAULT_SOURCE -s -O2

INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external -I$(RAYLIB_PATH)/src/extras

LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

LDFLAGS = -L. -L$(RAYLIB_PATH)/src

PROJECT_SOURCE_FILES ?= $(wildcard *.c)

OBJS = $(patsubst %.c, %.o, $(PROJECT_SOURCE_FILES))

MAKEFILE_PARAMS = $(BINARY)

all:
	mkdir -p ../bin
	$(MAKE) $(MAKEFILE_PARAMS)

$(BINARY): $(OBJS)
	$(CC) -o $(BINARY)$(EXT) $(OBJS) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D$(PLATFORM)
	rm -fv *.o

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM)

clean:
	find . -type f -executable -delete
	rm -fv *.o
	rm -rf ../bin
	@echo Cleaning done