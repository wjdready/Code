#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <inttypes.h> // PRId64

GLubyte *loadFile(const GLchar *path)
{
    SDL_RWops *file = SDL_RWFromFile(path, "rb");
    if (file == NULL)
    {
        SDL_Log("Could not open %s", path);
        return NULL;
    }

    Sint64 res_size = SDL_RWsize(file);
    if (res_size < 0 || res_size > 500 * 1024 * 1024)
    {
        SDL_Log("Invalid file size %s: %" PRId64, path, res_size);
        SDL_RWclose(file);
        return NULL;
    }

    GLubyte *ptr = SDL_malloc(res_size + 1);
    if (ptr == NULL)
    {
        SDL_Log("Could not allocate memory for %s", path);
        SDL_RWclose(file);
        return NULL;
    }

    Sint64 nb_read_total = 0, nb_read = 1;
    GLubyte *buf = ptr;
    while (nb_read_total < res_size && nb_read != 0)
    {
        nb_read = SDL_RWread(file, buf, 1, (res_size - nb_read_total));
        nb_read_total += nb_read;
        buf += nb_read;
    }

    SDL_RWclose(file);

    if (nb_read_total != res_size)
    {
        SDL_Log("Failed to read %s", path);
        SDL_free(ptr);
        return NULL;
    }

    ptr[res_size] = 0;
    return ptr;
}
