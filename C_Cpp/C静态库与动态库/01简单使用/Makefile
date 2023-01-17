# [MyProject]

default:
	gcc src/main.c 

run:
	./a.exe

clean:
	@rm ./a.exe 2> /dev/null || true

fullclean:
	@rm ./a.exe 2> /dev/null || true

static_lib_create:
	gcc main.c

dynamic_lib_create:
	gcc -shared add.c -o libadd.dll

