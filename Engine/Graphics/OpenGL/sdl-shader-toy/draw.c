#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define DECL(type, name) extern type name;
#define ODECL(type, name) extern type name;
#include "exts.h"
#undef DECL
#undef ODECL

extern GLuint shaderProgID;
extern GLuint vaoID;
extern GLuint vboID;
extern GLuint eboID;

extern GLint uResolution;
extern GLint uMouse;
extern GLint uTime;

extern GLfloat vertices[];
extern GLuint vCount;
extern GLint indices[];
extern GLuint iCount;

void glDraw(float time, int windowW, int windowH, int cursorX, int cursorY)
{
    glViewport(0, 0, windowW, windowH);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);

    if (uTime >= 0)
        glUniform1f(uTime, (GLfloat)(time));
    if (uResolution >= 0)
        glUniform2f(uResolution, (GLfloat)(windowW), (GLfloat)(windowH));
    if (uMouse >= 0)
        glUniform2f(uMouse, (GLfloat)(cursorX), (GLfloat)(cursorY));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(0));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}
