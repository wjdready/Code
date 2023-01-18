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
	cd build && cmake .. -G "MSYS Makefiles" && make -j8 

run:
	cd build && make -j8 && start main.exe

clean:
	rm  build -rf 2> /dev/null || true

