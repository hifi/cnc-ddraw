CC=i586-mingw32msvc-gcc
WINDRES=i586-mingw32msvc-windres
CFLAGS=-Wall -O3 -s -I./libsdl/include/SDL
LIBS=-lSDL -L./libsdl/lib/
REV=$(shell sh -c 'git rev-parse --short @{0}')
OBJS=main.o sdl.o hacks.o loader.o palette.o surface.o clipper.o

%.o: $*.c *.h
	$(CC) $(CFLAGS) -c -o $@ $*.c

ddraw: $(OBJS)
	sed 's/__REV__/$(REV)/g' ddraw.rc.in > ddraw.rc
	$(WINDRES) -J rc ddraw.rc ddraw.rc.o
	$(CC) $(CFLAGS) -Wl,--enable-stdcall-fixup -shared -o ddraw.dll $(OBJS) ddraw.def ddraw.rc.o $(LIBS)

all: ddraw

clean:
	rm -f *.o ddraw.dll
