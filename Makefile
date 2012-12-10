CC=gcc
CFLAGS=-DHAVE_LIBPNG -Iinclude -Wall -Wl,--enable-stdcall-fixup -O3 -s
LIBS=lib/libpng14.a lib/libz.a -lgdi32 -lopengl32

all:
	$(CC) $(CFLAGS) -shared -o ddraw.dll main.c mouse.c palette.c surface.c clipper.c render.c render_soft.c render_dummy.c screenshot.c ddraw.def $(LIBS)

clean:
	rm -f ddraw.dll
