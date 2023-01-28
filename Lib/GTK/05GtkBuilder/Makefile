# [MyProject]

default:
	gcc `pkg-config --cflags gtk4` main.c `pkg-config --libs gtk4`

mytry:
	gcc `pkg-config --cflags gtk4` mytry.c `pkg-config --libs gtk4`

run:
	./a.exe

clean:
	@rm ./a.exe 2> /dev/null || true 

fullclean:
	make clean

