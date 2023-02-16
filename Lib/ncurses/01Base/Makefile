# [MyProject]

default:
	@echo gcc aa.c `pkg-config ncursesw --libs --cflags --static` -static
	@gcc aa.c `pkg-config ncursesw --libs --cflags --static` -static 

run:
	@echo "We can not run this programe in terminal?"

clean:
	@rm ./a.exe 2> /dev/null || true 

globalclean:
	make clean 

fullclean:
	make clean
