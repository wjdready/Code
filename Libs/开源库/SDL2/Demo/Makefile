# [MyProject]


default:
	gcc main.c `pkg-config sdl2 --cflags --libs --static` -static

run: 
	./a.exe

clean:
	@rm ./a.exe build -r 2> /dev/null || true

cmake_build:
	mkdir build -p && cd build && cmake .. -G "MSYS Makefiles" && make -j8

fullclean:
	make clean
