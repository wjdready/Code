#include <SDL2/SDL.h>
#include <GLES2/gl2.h>
#include <iostream>
#include <cmath>

// 顶点着色器源代码
const char* vertexShaderSource = R"(
    attribute vec3 aPosition;
    attribute vec3 aColor;
    varying vec3 vColor;
    uniform mat4 uRotation;
    void main() {
        gl_Position = uRotation * vec4(aPosition, 1.0);
        vColor = aColor;
    }
)";

// 片段着色器源代码
const char* fragmentShaderSource = R"(
    precision mediump float;
    varying vec3 vColor;
    void main() {
        gl_FragColor = vec4(vColor, 1.0);
    }
)";

// 编译着色器函数
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // 检查编译错误
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "着色器编译失败: " << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

// 创建着色器程序
GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // 检查链接错误
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "程序链接失败: " << infoLog << std::endl;
        return 0;
    }

    // 链接完成后可以删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

int main(int argc, char* argv[]) {
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL初始化失败: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 设置SDL窗口属性为GLES 2.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // 创建窗口
    SDL_Window* window = SDL_CreateWindow(
        "GLES 2.0 示例",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "窗口创建失败: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // 创建GLES上下文
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "GLES上下文创建失败: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 创建着色器程序
    GLuint program = createShaderProgram();
    if (!program) {
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    glUseProgram(program);

    // 三角形顶点数据 (位置和颜色)
    float vertices[] = {
        // 位置         // 颜色
        0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 上顶点 - 红色
       -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下顶点 - 绿色
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // 右下顶点 - 蓝色
    };

    // 创建顶点缓冲对象
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 获取属性位置
    GLint aPosition = glGetAttribLocation(program, "aPosition");
    GLint aColor = glGetAttribLocation(program, "aColor");
    GLint uRotation = glGetUniformLocation(program, "uRotation");

    // 设置顶点属性指针
    glVertexAttribPointer(aPosition, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(aPosition);
    glVertexAttribPointer(aColor, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(aColor);

    // 设置清除颜色 (蓝色)
    glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

    bool running = true;
    float angle = 0.0f;

    // 主循环
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        // 计算旋转矩阵 (绕Z轴)
        angle += 0.5f;
        float rad = angle * M_PI / 180.0f;
        float rotation[16] = {
            cos(rad), -sin(rad), 0.0f, 0.0f,
            sin(rad),  cos(rad), 0.0f, 0.0f,
            0.0f,      0.0f,     1.0f, 0.0f,
            0.0f,      0.0f,     0.0f, 1.0f
        };
        glUniformMatrix4fv(uRotation, 1, GL_FALSE, rotation);

        // 清除屏幕
        glClear(GL_COLOR_BUFFER_BIT);

        // 绘制三角形
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 交换缓冲区
        SDL_GL_SwapWindow(window);

        // 控制帧率
        SDL_Delay(16);
    }

    // 清理资源
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(program);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
