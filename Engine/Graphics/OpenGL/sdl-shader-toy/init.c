#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define DECL(type, name) type name;
#define ODECL(type, name) type name;
#include "exts.h"
#undef DECL
#undef ODECL

extern GLfloat vertices[];
extern GLuint vCount;
extern GLint indices[];
extern GLuint iCount;

extern GLubyte *loadFile(const GLchar *path);
extern GLboolean glLoadExts();

GLuint shaderProgID = 0;
GLuint vaoID = 0;
GLuint vboID = 0;
GLuint eboID = 0;
GLint uResolution = -1;
GLint uMouse = -1;
GLint uTime = -1;

void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                     const GLchar *message, const void *userParam)
{
    GLchar *msgType = "unknown";
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        msgType = "error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        msgType = "deprecated";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        msgType = "undefined";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        msgType = "portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        msgType = "performance";
        break;
    case GL_DEBUG_TYPE_OTHER:
        msgType = "other";
        break;
    }

    GLchar *sevType = "unknown";
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_LOW:
        sevType = "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        sevType = "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        sevType = "HIGH";
        break;
    }

    SDL_Log("id: %d, type: %s, severity: %s: %s\n", id, msgType, sevType, message);
}

GLboolean isShaderCompiled(GLuint shaderID, GLchar *shaderName)
{
    GLint isCompiled = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

        GLchar *errorLog = (GLchar *)SDL_malloc(maxLength);
        if (errorLog == NULL)
        {
            SDL_Log("failed to compile %s, memory exhausted\n", shaderName);
            return GL_FALSE;
        }

        glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

        SDL_Log("failed to compile %s\n%s\n", shaderName, errorLog);

        SDL_free(errorLog);
        return GL_FALSE;
    }

    return GL_TRUE;
}

GLboolean isProgramLinked(GLuint programID)
{
    GLint isLinked = 0;
    glGetProgramiv(programID, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

        GLchar *errorLog = (GLchar *)SDL_malloc(maxLength);
        if (errorLog == NULL)
        {
            SDL_Log("failed to link program, memory exhausted\n");
            return GL_FALSE;
        }

        glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);

        SDL_Log("failed to link program\n%s\n", errorLog);

        SDL_free(errorLog);

        return GL_FALSE;
    }

    return GL_TRUE;
}

GLboolean glInit(GLchar *shaderName)
{
    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    const GLubyte *shading = glGetString(GL_SHADING_LANGUAGE_VERSION);

    SDL_Log("vendor:   %s\n", vendor);
    SDL_Log("renderer: %s\n", renderer);
    SDL_Log("version:  %s\n", version);
    SDL_Log("shading:  %s\n", shading);

    if (!glLoadExts())
        return GL_FALSE;

    if (glDebugMessageCallback)
    {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(openglCallbackFunction, NULL);
        GLuint unusedIds = 0;
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, GL_TRUE);
    }

    // int numExt = 0;
    // glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);
    // SDL_Log("extensions: (%d entries)\n", numExt);
    // for (int i = 0; i < numExt; i++) {
    //     const GLubyte *ext = glGetStringi(GL_EXTENSIONS, i);
    //     SDL_Log("\t%s\n", ext);
    // }

    const GLchar *vertShaderText = (const GLchar *)loadFile("tri.vert");
    if (vertShaderText == NULL)
    {
        return GL_FALSE;
    }

    GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShaderID, 1, &vertShaderText, NULL);
    glCompileShader(vertShaderID);
    if (!isShaderCompiled(vertShaderID, "tri.vert"))
    {
        glDeleteShader(vertShaderID);
        return GL_FALSE;
    }

    const GLchar *fragShaderText = (const GLchar *)loadFile(shaderName);
    if (fragShaderText == NULL)
    {
        return GL_FALSE;
    }
    GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShaderID, 1, &fragShaderText, NULL);
    glCompileShader(fragShaderID);
    if (!isShaderCompiled(fragShaderID, shaderName))
    {
        glDeleteShader(vertShaderID);
        glDeleteShader(fragShaderID);
        return GL_FALSE;
    }

    shaderProgID = glCreateProgram();
    glAttachShader(shaderProgID, vertShaderID);
    glAttachShader(shaderProgID, fragShaderID);
    glLinkProgram(shaderProgID);
    if (!isProgramLinked(shaderProgID))
    {
        glDeleteShader(vertShaderID);
        glDeleteShader(fragShaderID);
        glDeleteProgram(shaderProgID);
        return GL_FALSE;
    }

    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, vCount * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &eboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, iCount * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glUseProgram(shaderProgID);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    uResolution = glGetUniformLocation(shaderProgID, "iResolution");
    if (uResolution < 0)
    {
        SDL_Log("shading:  could not find iResolution uniform\n");
    }
    uMouse = glGetUniformLocation(shaderProgID, "iMouse");
    if (uResolution < 0)
    {
        SDL_Log("shading:  could not find iMouse uniform\n");
    }
    uTime = glGetUniformLocation(shaderProgID, "iTime");
    if (uResolution < 0)
    {
        SDL_Log("shading:  could not find iTime uniform\n");
    }

    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return GL_TRUE;
}
