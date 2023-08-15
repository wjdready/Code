
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

# -----------------------------------------------------
# include 部分

squote  := '

# 转义单引号以在 echo 语句中使用
escsq = $(subst $(squote),'\$(squote)',$1)

# 命令执行时回显
# 如果有自定义回显内容 $(quiet)cmd_$(1), 则执行回显
echo-cmd = $(if $($(quiet)cmd_$(1)),\
	echo '  $(call escsq,$($(quiet)cmd_$(1)))$(echo-why)';)

# sink stdout for 'make -s'
       redirect :=
 quiet_redirect :=
silent_redirect := exec >/dev/null;

# 执行一个命令
# 通过 $(call cmd, name) 来执行, 它会根据需要将命令回显，同时执行实际命令 cmd_name
cmd = @set -e; $(echo-cmd) $($(quiet)redirect) $(cmd_$(1))

# why - 告诉 User 为何该目标需要进行构建
# 当 V=2 时才显示
ifeq ($(KBUILD_VERBOSE),2)
why = $(if $(0), 						\
			- 'due to something'	    \
		)
endif

echo-why = $(call escsq, $(strip $(why)))
# -----------------------------------------------------

# 定义两个命令, 一个是实际命令, 一个是该命令调用时的自定义显示
quiet_cmd_hello = GEN $@
      cmd_hello = echo hello

all: tar1
	$(call cmd,hello)

tar1:
	$(Q)echo "tar1"


