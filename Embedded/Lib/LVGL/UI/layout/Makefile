# [MyProject]

default:
    ifeq ("$(wildcard ../../_tmp/lvgl)", "")
	@echo "lvgl sim does not exist."
	@echo "Perform download..."
	@git clone https://github.com/lvgl/lv_port_pc_eclipse.git ../_tmp/lvgl
	@cd ../_tmp/lvgl && git submodule init && git submodule update
    endif

	make cmake_build


cmake_build:
	cmake -B build -G "MSYS Makefiles" && make -C build -j8 

run:
	cd build && make -j8 && start main.exe

clean:
	rm  build -rf 2> /dev/null || true

globalclean:
	make clean 
	rm lvgl -rf 

