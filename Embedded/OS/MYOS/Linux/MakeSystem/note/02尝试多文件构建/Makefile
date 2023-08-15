# SPDX-License-Identifier: GPL-2.0
VERSION = 5
PATCHLEVEL = 18
SUBLEVEL = 2
EXTRAVERSION =
NAME = Superb Owl

# 当命令行上没有给出任何信息时，这是我们的默认目标
PHONY := __all
__all:

# 不使用 make 的内置规则和变量(这样做可提高性能并避免难以调试的行为)
MAKEFLAGS += -rR

srctree := $(PWD)

export building_out_of_srctree srctree objtree VPATH

CROSS_COMPILE = arm-none-eabi-
CC		= $(CROSS_COMPILE)gcc
LD		= $(CROSS_COMPILE)ld
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump
READELF		= $(CROSS_COMPILE)readelf
STRIP		= $(CROSS_COMPILE)strip

KBUILD_LDFLAGS := 

export ARCH SRCARCH CONFIG_SHELL BASH HOSTCC KBUILD_HOSTCFLAGS CROSS_COMPILE LD CC
export PERL PYTHON3 CHECK CHECKFLAGS MAKE UTS_MACHINE HOSTCXX

# Beautify output
# ---------------------------------------------------------------------------
#
# Normally, we echo the whole command before executing it. By making
# that echo $($(quiet)$(cmd)), we now have the possibility to set
# $(quiet) to choose other forms of output instead, e.g.
#
#         quiet_cmd_cc_o_c = Compiling $(RELDIR)/$@
#         cmd_cc_o_c       = $(CC) $(c_flags) -c -o $@ $<
#
# If $(quiet) is empty, the whole command will be printed.
# If it is set to "quiet_", only the short version will be printed.
# If it is set to "silent_", nothing will be printed at all, since
# the variable $(silent_cmd_cc_o_c) doesn't exist.
#
# A simple variant is to prefix commands with $(Q) - that's useful
# for commands that shall be hidden in non-verbose mode.
#
#	$(Q)ln $@ :<
#
# If KBUILD_VERBOSE equals 0 then the above command will be hidden.
# If KBUILD_VERBOSE equals 1 then the above command is displayed.
# If KBUILD_VERBOSE equals 2 then give the reason why each target is rebuilt.
#
# To put more focus on warnings, be less verbose as default
# Use 'make V=1' to see the full commands

ifeq ("$(origin V)", "command line")
  KBUILD_VERBOSE = $(V)
endif
ifndef KBUILD_VERBOSE
  KBUILD_VERBOSE = 0
endif

ifeq ($(KBUILD_VERBOSE),1)
  quiet =
  Q =
else
  quiet=quiet_
  Q = @
endif

# If the user is running make -s (silent mode), suppress echoing of
# commands

ifneq ($(findstring s,$(filter-out --%,$(MAKEFLAGS))),)
  quiet=silent_
  KBUILD_VERBOSE = 0
endif

export quiet Q KBUILD_VERBOSE


# ---------------------------------------------------------------------------
# 导出 OBJS 

core-y			+= kernel

# Externally visible symbols (used by link-vmlinux.sh)
KBUILD_VMLINUX_OBJS := gg.o

export KBUILD_VMLINUX_OBJS

# Cross compiling and selecting different set of gcc/bin-utils
# ---------------------------------------------------------------------------

# SHELL used by kbuild
CONFIG_SHELL := sh

# 使用 make M=dir 或设置环境变量 KBUILD_EXTMOD 来指定要构建的外部模块的目录,
# 两者都都设置的情况下，优先采用 M=dir 的设置内容
ifeq ("$(origin M)", "command line")
  KBUILD_EXTMOD := $(M)
endif

# 不支持同时构建多个外部模块
$(if $(word 2, $(KBUILD_EXTMOD)), \
	$(error building multiple external modules is not supported))

# 删除尾部斜杠
ifneq ($(filter %/, $(KBUILD_EXTMOD)),)
KBUILD_EXTMOD := $(shell dirname $(KBUILD_EXTMOD).)
endif

export KBUILD_EXTMOD


# ===========================================================================
# 构建目标, 包括 vmlinux, arch specific targets, clean targets and others.
# In general all targets except *config targets.
# If building an external module we do not care about the all: rule
# but instead __all depend on modules

include $(srctree)/scripts/Kbuild.include

PHONY += all
ifeq ($(KBUILD_EXTMOD),)
__all: all
else
__all: modules
endif

PHONY += autoksyms_recursive
ifdef CONFIG_TRIM_UNUSED_KSYMS
# For the kernel to actually contain only the needed exported symbols,
# we have to build modules as well to determine what those symbols are.
# (this can be evaluated only once include/config/auto.conf has been included)
KBUILD_MODULES := 1

autoksyms_recursive: descend modules.order
	$(Q)$(CONFIG_SHELL) $(srctree)/scripts/adjust_autoksyms.sh \
		"$(MAKE) -f $(srctree)/Makefile vmlinux"
endif

all: vmlinux modpost

vmlinux: scripts/link-vmlinux.sh autoksyms_recursive $(vmlinux-deps) FORCE
	+$(call if_changed_dep,link-vmlinux)

# 最终链接成 vmlinux 文件,  最终环节后有一个可选的 ARCH_POSTLINK
cmd_link-vmlinux =                                                 \
	$(CONFIG_SHELL) $< "$(LD)" "$(KBUILD_LDFLAGS)" "$(LDFLAGS_vmlinux)";    \
	$(if $(ARCH_POSTLINK), $(MAKE) -f $(ARCH_POSTLINK) $@, true)

PHONY += descend $(build-dirs)
descend: $(build-dirs)
$(build-dirs): prepare
	$(Q)$(MAKE) $(build)=$@ \
	single-build=$(if $(filter-out $@/, $(filter $@/%, $(KBUILD_SINGLE_TARGETS))),1) \
	need-builtin=1 need-modorder=1

modules.order: $(subdir-modorder) FORCE
	$(call if_changed,modules_order)

# ---------------------------------------------------------------------------
# Modules

PHONY += modules modules_install

ifdef CONFIG_MODULES

modules: modules_check
	$(Q)$(MAKE) -f $(srctree)/scripts/Makefile.modpost

PHONY += modules_check
modules_check: $(MODORDER)
	$(Q)$(CONFIG_SHELL) $(srctree)/scripts/modules-check.sh $<

quiet_cmd_depmod = DEPMOD  $(MODLIB)
      cmd_depmod = $(CONFIG_SHELL) $(srctree)/scripts/depmod.sh $(DEPMOD) \
                   $(KERNELRELEASE)

modules_install:
	$(Q)$(MAKE) -f $(srctree)/scripts/Makefile.modinst
	$(call cmd,depmod)

else # CONFIG_MODULES

# Modules not configured
# ---------------------------------------------------------------------------

modules modules_install:
	@echo >&2 '***'
	@echo >&2 '*** The present kernel configuration has modules disabled.'
	@echo >&2 '*** To use the module feature, please run "make menuconfig" etc.'
	@echo >&2 '*** to enable CONFIG_MODULES.'
	@echo >&2 '***'
	@exit 1

endif # CONFIG_MODULES

# Other target
# ---------------------------------------------------------------------------

clean:
	rm vmlinux modules 2> /dev/null || true


PHONY += FORCE
FORCE:

# Declare the contents of the PHONY variable as phony.  We keep that
# information in a variable so we can use it in if_changed and friends.
.PHONY: $(PHONY)

