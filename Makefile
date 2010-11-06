all:
	i586-mingw32msvc-gcc -Wall -Wl,--enable-stdcall-fixup -shared -s -o ddraw.dll main.c mouse.c palette.c surface.c clipper.c render_ddraw.c render_opengl.c ddraw.def -lgdi32 -lopengl32

clean:
	rm -f ddraw.dll
