all:
	i586-mingw32msvc-gcc -D_DEBUG -Wall -Wl,--enable-stdcall-fixup -shared -s -o ddraw.dll main.c palette.c surface.c clipper.c ddraw.def -lgdi32 -lopengl32

clean:
	rm -f ddraw.dll
