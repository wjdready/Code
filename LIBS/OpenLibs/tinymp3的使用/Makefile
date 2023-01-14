
target = test.exe

default:
    ifeq ("$(wildcard tinymp3)", "")
	@echo "tinymp3 does not exist."
	@echo "Perform download..."
	@git clone https://github.com/cpuimage/tinymp3.git
    endif

	make build

run: 
	./$(target)

build:
	gcc main.c tinymp3/shine_mp3.c -Itinymp3 -o $(target)

clean:
	rm test.mp3 $(target) 2> /dev/null || true

fullclean:
	make clean 2> /dev/null || true
	rm ./tinymp3 -rf 2> /dev/null || true
