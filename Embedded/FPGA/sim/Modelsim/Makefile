


all:

# 创建一个工作 lib, 可以不事先创建, vlog 时也会创建
# vlib work

# 编译, 默认 work 文件夹, 不存在时会自动创建, 可指定 -work mywork
	vlog demo.v

# 运行仿真程序并指明 top 层
	vsim -voptargs=+acc top -do "add wave sim:/top/*; run 10ms;"

# 运行地同时执行 tcl 指令, 可在 GUI 界面观察然后粘贴到此
# vsim -voptargs=+acc work.top -do "add wave sim:/top/*; run 10ms;"

# 运行然后退出 -c 不打开 GUI, 可用来生成波形或验证
# vsim -voptargs=+acc work.top -do "run 10ms; exit;" -c

# 添加某个 lib
# vsim -L GW5A -Lf GW5A work.testbench {-voptargs=+acc -L GW5A -Lf GW5A}

top = crc32_tb

default:
	vlog crc32.v crc32_tb.v
	vsim -voptargs=+acc -L GW5A -Lf GW5A $(top) -do "add wave sim:/$(top)/*; run 10ms;"
