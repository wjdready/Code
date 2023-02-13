# [MyProject]

default:
	make uic
	mkdir build -p && cd build && qmake .. && make -j8

run:
	./build/debug/TestSerialPort.exe

uic:
	uic resource/ui/MainWidget.ui > resource/ui/ui_MainWidget.h

clean:
	@rm build -rf 2> /dev/null || true

globalclean:
	@rm build -rf 2> /dev/null || true

