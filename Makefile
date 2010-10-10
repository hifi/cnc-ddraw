all:
	i586-mingw32msvc-gcc -Wall -Wl,--enable-stdcall-fixup -shared -s -o ddraw.dll main.c palette.c surface.c ddraw.def

clean:
	rm -f ddraw.dll
