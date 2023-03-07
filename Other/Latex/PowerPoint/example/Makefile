
BUILD_DIR = build

SRCS += src/demo.md

default:
# ./build.sh $(SRCS)
	mkdir build -p
	pdflatex -output-dir=build src/mydemo.tex

clean:
	rm  build -rf 2> /dev/null || true
