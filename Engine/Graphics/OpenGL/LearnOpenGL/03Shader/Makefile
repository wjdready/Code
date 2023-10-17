
GLAD_DIR = ../../glad

SRCS += $(GLAD_DIR)/src/glad.c
INCS += -I$(GLAD_DIR)/include

# SRCS += 渐变三角形.cpp
# SRCS += 更多顶点属性.cpp
SRCS += shaders_class2.cpp

# -lmingw32 -lSDL2main -lSDL2 -lglmf32 -lgdi32 -static
LIBS += -lopengl32 -lglfw3 -lgdi32  -static

INCS  += -I.

# -mwindows
CFLAGS += -Wall $(INCS) $(LIBS) 

all:
	g++ $(SRCS) $(CFLAGS)
