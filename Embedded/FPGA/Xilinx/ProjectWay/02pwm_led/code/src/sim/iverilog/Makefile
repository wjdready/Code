

SRC_DIR = ../../utility/hdl

default:
	iverilog -I$(SRC_DIR) test.v
	vvp ./a.out && gtkwave wave.vcd

clean:
	rm a.out wave.vcd -rf > /dev/null
