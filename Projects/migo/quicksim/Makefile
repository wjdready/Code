# [MyProject]

default:
    ifeq ("$(wildcard lvgl)", "")
	@echo "lvgl sim does not exist."
	@echo "Perform download..."
	@git clone https://github.com/lvgl/lv_port_pc_eclipse.git lvgl
	@cd lvgl && git submodule init && git submodule update
    endif

	make cmake_build

cmake_build:
	mkdir -p build
	cmake -B build -G "Unix Makefiles" && make -C build -j8 

run:
	make -C build -j8 && start build/main.exe

clean:
	rm  build -rf 2> /dev/null || true

globalclean:
	make clean 

fullclean:
	make clean 
	rm lvgl -rf
