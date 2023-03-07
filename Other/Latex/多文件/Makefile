
BUILD_DIR = build

SRCS += src/note.tex

default:
	mkdir -p $(BUILD_DIR)
	xelatex -output-dir=$(BUILD_DIR) $(SRCS)

clean:
	rm  build -rf 2> /dev/null || true

