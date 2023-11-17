# Makefile

TOPLEVEL_LANG = verilog

SIM ?= icarus
WAVES ?= 0

COCOTB_HDL_TIMEUNIT = 1ns
COCOTB_HDL_TIMEPRECISION = 1ps

DUT      = dff
TOPLEVEL = $(DUT)
MODULE   = test_$(DUT)

VERILOG_SOURCES = dff.v

ifeq ($(SIM), icarus)
	PLUSARGS += -fst

#	COMPILE_ARGS += -P $(TOPLEVEL).A=$(PARAM_A)

	ifeq ($(WAVES), 1)
		VERILOG_SOURCES += iverilog_dump.v
		COMPILE_ARGS += -s iverilog_dump
	endif
else ifeq ($(SIM), verilator)
	COMPILE_ARGS += -Wno-SELRANGE -Wno-WIDTH

#	COMPILE_ARGS += -GA=$(PARAM_A)

	ifeq ($(WAVES), 1)
		COMPILE_ARGS += --trace-fst
	endif
endif

include $(shell cocotb-config --makefiles)/Makefile.sim

iverilog_dump.v:
	echo 'module iverilog_dump();' > $@
	echo 'initial begin' >> $@
	echo '    $$dumpfile("$(TOPLEVEL).fst");' >> $@
	echo '    $$dumpvars(0, $(TOPLEVEL));' >> $@
	echo 'end' >> $@
	echo 'endmodule' >> $@

$(TOPLEVEL).fst:
	make WAVES=1

gtkwave: $(TOPLEVEL).fst
	gtkwave $(TOPLEVEL).fst

clean::
	@rm -rf iverilog_dump.v
	@rm -rf dump.fst $(TOPLEVEL).fst
	@rm -rf __pycache__
	@rm -rf results.xml
