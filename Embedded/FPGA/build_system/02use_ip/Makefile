BUILD_DIR = build

# FPGA settings
FPGA_PART = xc7z010clg400-1
FPGA_TOP = system_wrapper

# Files for synthesis
SYN_FILES += top.v

# XDC files
XDC_FILES += pin.xdc

# IP
IP_REPO_PATHS += ../ip_core
XCI_FILES += led_0.xci

# BD
BD_TCL_FILES += system_bd.tcl

# Config
# CONFIG_TCL_FILES = ./config.tcl

include ../scripts/vivado.mk

