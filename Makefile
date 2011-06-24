CC=i586-mingw32msvc-gcc
WINDRES=i586-mingw32msvc-windres
CFLAGS=-Iinclude -Wall -Wl,--enable-stdcall-fixup -O3 -s -Ilibsdl/include/SDL
LIBS=-lSDL -Llibsdl/lib/
REV=$(shell sh -c 'git rev-parse --short @{0}')

all:
	sed 's/__REV__/$(REV)/g' ddraw.rc.in > ddraw.rc
	$(WINDRES) -J rc ddraw.rc ddraw.rc.o
	$(CC) $(CFLAGS) -shared -o ddraw.dll main.c sdl.c mouse.c palette.c surface.c clipper.c ddraw.def ddraw.rc.o $(LIBS)

clean:
	rm -f ddraw.dll
