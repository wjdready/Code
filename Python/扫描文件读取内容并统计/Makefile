# [MyProject]

test_file = example.xml

default run:
	python demo.py $(test_file)

clean:
	@rm output.txt 2> /dev/null || true
	@rm dist build -rf 2> /dev/null || true
	@rm demo.spec 2> /dev/null || true

build_exe:
	pyinstaller --onefile --nowindowed  demo.py

globalclean:
	make clean
