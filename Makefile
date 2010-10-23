all:
	i586-mingw32msvc-gcc -Wall -Wl,--enable-stdcall-fixup -shared -s -o ddraw.dll main.c mouse.c palette.c surface.c clipper.c ddraw.def -lgdi32

clean:
	rm -f ddraw.dll
