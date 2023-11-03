# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "CLK_HZ" -parent ${Page_0}
  ipgui::add_param $IPINST -name "CNT_1S" -parent ${Page_0}
  ipgui::add_param $IPINST -name "PER" -parent ${Page_0}


}

proc update_PARAM_VALUE.CLK_HZ { PARAM_VALUE.CLK_HZ } {
	# Procedure called to update CLK_HZ when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.CLK_HZ { PARAM_VALUE.CLK_HZ } {
	# Procedure called to validate CLK_HZ
	return true
}

proc update_PARAM_VALUE.CNT_1S { PARAM_VALUE.CNT_1S } {
	# Procedure called to update CNT_1S when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.CNT_1S { PARAM_VALUE.CNT_1S } {
	# Procedure called to validate CNT_1S
	return true
}

proc update_PARAM_VALUE.PER { PARAM_VALUE.PER } {
	# Procedure called to update PER when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.PER { PARAM_VALUE.PER } {
	# Procedure called to validate PER
	return true
}


proc update_MODELPARAM_VALUE.CLK_HZ { MODELPARAM_VALUE.CLK_HZ PARAM_VALUE.CLK_HZ } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.CLK_HZ}] ${MODELPARAM_VALUE.CLK_HZ}
}

proc update_MODELPARAM_VALUE.CNT_1S { MODELPARAM_VALUE.CNT_1S PARAM_VALUE.CNT_1S } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.CNT_1S}] ${MODELPARAM_VALUE.CNT_1S}
}

proc update_MODELPARAM_VALUE.PER { MODELPARAM_VALUE.PER PARAM_VALUE.PER } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.PER}] ${MODELPARAM_VALUE.PER}
}

