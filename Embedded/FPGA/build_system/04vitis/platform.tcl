# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct C:\Users\shino\Desktop\AWA4051\makeAWA4051\demo_axi\vitis\system_wrapper\platform.tcl
# 
# OR launch xsct and run below command.
# source C:\Users\shino\Desktop\AWA4051\makeAWA4051\demo_axi\vitis\system_wrapper\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {system_wrapper} \
-hw {build/system_wrapper.xsa}    \
-out {vitis}

platform write
domain create -name {standalone_ps7_cortexa9_0} -display-name {standalone_ps7_cortexa9_0} -os {standalone} -proc {ps7_cortexa9_0} -runtime {cpp} -arch {32-bit} -support-app {hello_world}
platform generate -domains 
platform active {system_wrapper}
domain active {zynq_fsbl}
domain active {standalone_ps7_cortexa9_0}
platform generate -quick
platform generate
platform active {system_wrapper}
domain create -name {standalone_ps7_cortexa9_1} -display-name {standalone_ps7_cortexa9_1} -os {standalone} -proc {ps7_cortexa9_1} -runtime {cpp} -arch {32-bit} -support-app {hello_world}
platform generate -domains 
platform write
domain active {zynq_fsbl}
domain active {standalone_ps7_cortexa9_0}
domain active {standalone_ps7_cortexa9_1}
platform generate -quick
platform generate -domains standalone_ps7_cortexa9_1 
domain active {zynq_fsbl}
bsp reload
domain active {standalone_ps7_cortexa9_0}
bsp reload
domain active {zynq_fsbl}
bsp reload
platform generate -domains 
platform generate -domains 
platform clean
platform generate
domain active {standalone_ps7_cortexa9_0}
bsp reload
domain active {standalone_ps7_cortexa9_0}
bsp reload
domain active {zynq_fsbl}
domain active {standalone_ps7_cortexa9_0}
platform generate -domains 
domain active {zynq_fsbl}
domain active {standalone_ps7_cortexa9_0}
domain active {zynq_fsbl}
catch {bsp regenerate}
platform generate -domains zynq_fsbl 
platform generate -domains 
bsp reload
domain active {standalone_ps7_cortexa9_0}
bsp reload
domain active {zynq_fsbl}
