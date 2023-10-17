
GLAD_DIR = ../../glad

SRCS += $(GLAD_DIR)/src/glad.c
INCS += -I$(GLAD_DIR)/include

SRCS += Textures.cpp

# -lmingw32 -lSDL2main -lSDL2 -lglmf32 -lgdi32 -static
LIBS += -lopengl32 -lglfw3 -L_local/lib -lstb-image 

INCS  += -I. -I_local/include

# -mwindows
CFLAGS += -Wall $(INCS) $(LIBS) 

all:
	g++ $(SRCS) $(CFLAGS)

stb_install:
    ifeq ("$(wildcard _local/.cache/stb-cmake)", "")
	@echo "Perform download..."
	@git clone https://github.com/gracicot/stb-cmake.git _local/.cache/stb-cmake
	cd _local/.cache/stb-cmake && \
	git submodule init && \
	git submodule update && \
	cmake -DCMAKE_INSTALL_PREFIX=../.. -B build && \
	ninja -C build && ninja -C build install
    endif
