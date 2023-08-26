


all:
	dtc demo.dts -o demo.dto

dts: demo.dto
	dtc -I dtb -O dts $< > __$<.dts

clean:
	rm *.dto _*
