

VIVADO = /c/ProgramFiles/Xilinx/Vivado/2022.2/bin/vivado.bat

FLAGS += -nojournal -nolog -tempDir build

default:
	$(VIVADO) $(FLAGS) -mode batch  -source scripts/flow.tcl

gui:
	$(VIVADO) $(FLAGS) -mode gui  -source scripts/flow.tcl -tclargs gui

sim:
	$(VIVADO) $(FLAGS) -mode batch -source scripts/flow.tcl -tclargs sim

cmd_mode:
	$(VIVADO) $(FLAGS) -mode tcl 

flash:
	$(VIVADO) $(FLAGS) -mode batch  -source scripts/flash.tcl

clean:
	rm build -rf > /dev/null
