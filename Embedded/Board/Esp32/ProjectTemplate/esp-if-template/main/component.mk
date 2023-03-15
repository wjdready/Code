#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

COMPONENT_ADD_INCLUDEDIRS := .
COMPONENT_SRCS := func.cpp main.cpp

# CFLAGS += -Wno-error=char-subscripts -Wno-error=attributes