

mysdl2_dynamic_lib: 
	gcc  mysdl2.c -shared -lSDL2 -o libmysdl2.dll
	gcc main.c -L. -lmysdl2

mysdl2_static_lib: 
	gcc -c mysdl2.c -o mysdl2.o
	ar -crv libmysdl2.a mysdl2.o
	gcc main.c -L. -lmysdl2 `pkg-config sdl2 --cflags --libs -static` -static -o static_main


clean:
	rm libmysdl2.* *.exe *.o 2> /dev/null || true