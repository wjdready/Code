

```sh
# digilent-hs1.cfg or digilent_jtag_smt2.cfg
source [find interface/ftdi/digilent_jtag_smt2.cfg] 

reset_config srst_only srst_push_pull

source [find target/zynq_7000.cfg]
# telnet_port 4440
# gdb_port 0
# tcl_port 0

# init
# zynqpl_program zynq_pl.bs

# zynqpl_program -help
ftdi tdo_sample_edge falling 
adapter speed 1000000
# pld -help
# pld load 0 BOOT.bin
# exit

# program BOOT.bin 0x08000000 verify

    
targets zynq.cpu0
init
reset init

# https://openocd.org/doc-release/html/PLD_002fFPGA-Commands.html
pld load 0 led.bit

# flash probe 0
# flash banks
# flash bank qspi cfi 0xFC000000 0x1000000 2 2 zynq.cpu0

# program  build/program/flash_program.elf verify

# openocd -f interface.cfg -f target/zynq_7000.cfg  -c "init" -c "zynqpl_program zynq_pl.bs" -c "pld load 0 sds1104xe.bit" -c "exit"

# add_xc_board(
#   BOARD zybo-z7-20
#   DEVICE xc7z020
#   PACKAGE test
#   PART xc7z020clg400-1
#   PROG_CMD "${OPENOCD} -f ${OPENOCD_DATADIR}/scripts/interface/ftdi/digilent-hs1.cfg -f
#    ${OPENOCD_DATADIR}/scripts/target/zynq_7000.cfg 
#    -c \\\"ftdi_tdo_sample_edge falling $<SEMICOLON> adapter_khz 1000000 $<SEMICOLON> init $<SEMICOLON> pld load 0 \${OUT_BIN} $<SEMICOLON> exit\\\""
# )
```