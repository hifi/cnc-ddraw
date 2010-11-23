CC=i586-mingw32msvc-gcc
WINDRES=i586-mingw32msvc-windres
CFLAGS=-DHAVE_LIBPNG -Iinclude -Wall -Wl,--enable-stdcall-fixup -Os -s
LIBS=lib/libpng14.a lib/libz.a -lgdi32 -lopengl32
REV=$(shell sh -c 'git rev-parse --short @{0}')

all:
	sed 's/__REV__/$(REV)/g' ddraw.rc.in > ddraw.rc
	$(WINDRES) -J rc ddraw.rc ddraw.rc.o
	$(CC) $(CFLAGS) -shared -o ddraw.dll main.c mouse.c palette.c surface.c clipper.c render.c screenshot.c ddraw.def ddraw.rc.o $(LIBS)

clean:
	rm -f ddraw.dll
