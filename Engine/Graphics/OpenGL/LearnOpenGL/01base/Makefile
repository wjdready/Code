
GLAD_DIR = ../../glad

SRCS += $(GLAD_DIR)/src/glad.c
INCS += -I$(GLAD_DIR)/include

SRCS += hello_glfw.cpp

# -lmingw32 -lSDL2main -lSDL2 -lglmf32 -lgdi32 -static
LIBS += -lopengl32 -lglfw3

INCS  += -I.

CFLAGS += -Wall $(INCS) $(LIBS) 

all:
	g++ $(SRCS) $(CFLAGS)
