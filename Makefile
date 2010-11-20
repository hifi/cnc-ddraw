all:
	i586-mingw32msvc-gcc -DHAVE_LIBPNG -Iinclude -Wall -Wl,--enable-stdcall-fixup -shared -s -o ddraw.dll main.c mouse.c palette.c surface.c clipper.c render.c screenshot.c lib/libpng14.a lib/libz.a ddraw.def -lgdi32 -lopengl32

clean:
	rm -f ddraw.dll
