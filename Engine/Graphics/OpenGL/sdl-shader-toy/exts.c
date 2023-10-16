#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

GLboolean glLoadExts()
{
#define DECL(type, name)                                                                                               \
    extern type name;                                                                                                  \
    name = (type)SDL_GL_GetProcAddress(#name);                                                                         \
    if (name == NULL)                                                                                                  \
    {                                                                                                                  \
        SDL_Log(" - could not find " #name "\n");                                                                      \
        return GL_FALSE;                                                                                               \
    }
#define ODECL(type, name)                                                                                              \
    extern type name;                                                                                                  \
    name = (type)SDL_GL_GetProcAddress(#name);                                                                         \
    if (name == NULL)                                                                                                  \
    {                                                                                                                  \
        SDL_Log(" - could not find " #name "\n");                                                                      \
    }
#include "exts.h"
#undef DECL
#undef ODECL
    return GL_TRUE;
}
