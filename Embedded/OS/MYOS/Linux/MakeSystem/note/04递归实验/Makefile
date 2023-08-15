
var := BAD
this-makefile := $(lastword $(MAKEFILE_LIST))

all:
	@echo $(var) $(sub_make_done)
	@$(if $(flag), $(MAKE) -f $(this-makefile))

ifneq ($(sub_make_done), 1)
var := GOOD
export sub_make_done := 1
flag := 1
# else
# # 第二次进来时, 有这个伪目标, 因而不再执行后面代码? 但是 make demo 是指定目标, 肯定会执行的
# $(this-makefile): ;
endif

demo:
	@echo "demo" $(sub_make_done)
	@$(if $(flag), $(MAKE) -f $(this-makefile))
