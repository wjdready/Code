#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

GLfloat vertices[] = {
	-1.f, -1.f,
	+1.f, -1.f,
	+1.f, +1.f,
	-1.f, +1.f,
};
GLuint vCount = 4 * 3; // 8 * 3;

GLint indices[] = {
    0, 2, 1,
    3, 2, 0,
};
GLuint iCount = 2 * 3;
