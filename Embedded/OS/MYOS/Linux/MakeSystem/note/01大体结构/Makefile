# 当命令行上没有给出任何信息时，这是我们的默认目标
PHONY := __all
__all:

PHONY += all
ifeq ($(KBUILD_EXTMOD),)
__all: all
else
__all: modules
endif

# ---------------------------------------------------------------------------
# vmlinux

all: vmlinux

vmlinux:
	@touch vmlinux
	@echo BUILD vmlinux

# ---------------------------------------------------------------------------
# Modules
ifdef CONFIG_MODULES

# 默认情况下，构建 vmlinux 的同时还会构建 modules 
all: modules

modules:
	@touch modules
	@echo BUILD modules

else # 用户没有使能 CONFIG_MODULES 时, 提示信息

modules modules_install:
	@echo >&2 '***'
	@echo >&2 '*** The present kernel configuration has modules disabled.'
	@echo >&2 '*** To use the module feature, please run "make menuconfig" etc.'
	@echo >&2 '*** to enable CONFIG_MODULES.'
	@echo >&2 '***'
	@exit 1

endif # CONFIG_MODULES


# Tools
# ---------------------------------------------------------------------------

clean:
	rm vmlinux modules 2> /dev/null || true

