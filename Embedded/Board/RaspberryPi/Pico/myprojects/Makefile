BUILD_PROJ=template

export PICO_SDK_PATH = $(PWD)/../_sdk/pico-sdk
export PICO_NO_GC_SECTIONS

default:
	cmake -B build -G "MSYS Makefiles"

$(BUILD_PROJ):
	make -C build/$(BUILD_PROJ) -j

clean:
	rm build -rf

.PHONY: $(BUILD_PROJ)
