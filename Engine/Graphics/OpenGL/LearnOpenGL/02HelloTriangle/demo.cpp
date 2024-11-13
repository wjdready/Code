#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// 初始宽度高度
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = R"(

#version 330 core

// 声明获取输入顶点的位置属性
layout (location = 0) in vec3 aPos;

void main()
{
    // gl_Position 是OpenGL中的一个内置变量，用于存储顶点经过所有必要的变换后的最终位置
    // 以齐次坐标表示。它表示顶点在经过模型、视图和投影等变换后的位置
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
};

)";

const char *fragmentShaderSource = R"(

#version 330 core

// 定义片段着色器的输出, 或者直接输出到 gl_FragColor 内置变量
out vec4 FragColor;

void main()
{
    // 这里总是输出单一颜色
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
};

)";


int main()
{
    // 初始化和配置 glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw 窗口创建
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: 加载所有 OpenGL 函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    printf("GL_VENDOR  : %s\n", glGetString(GL_VENDOR) );
    printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER) );
    printf("GL_VERSION : %s\n", glGetString(GL_VERSION) );

    // 编译着色器程序

    // 1. vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // 检查 shader 编译是否出错
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 2. fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // 检查 shader 编译是否出错
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 3. 链接 shader
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // 检查链接是否出错
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 设置折点数据(和缓冲区(S))并配置折点属性
    // 由于我们希望渲染一个三角形，我们一共要指定三个顶点，每个顶点都有一个3D位置。
    // 我们会将它们以标准化设备坐标的形式（OpenGL的可见区域）定义为一个float数组
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    // 顶点缓冲对象 (Vertex Buffer Objects, VBO) 用于在 GPU 内存（通常被称为显存）中储存大量顶点
    // 顶点数组对象 (Vertex Array Object, VAO) 可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中。
    // 元素缓冲对象(Element Buffer Object，EBO)
    unsigned int VBO, VAO, EBO;

    // 就像OpenGL中的其它对象一样，这个缓冲有一个独一无二的ID，所以我们可以使用glGenBuffers函数和一个缓冲ID生成一个VBO对象
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    // 首先绑定顶点数组对象，然后绑定并设置顶点缓冲区，然后配置顶点属性
    glBindVertexArray(VAO);

    // OpenGL 有很多缓冲对象类型，顶点缓冲对象的缓冲类型是 GL_ARRAY_BUFFER
    // OpenGL 允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型
    // 我们可以使用 glBindBuffer 函数把新创建的缓冲绑定到 GL_ARRAY_BUFFER 目标上
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 从这一刻起，我们使用的任何（在 GL_ARRAY_BUFFER 目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。
    // 然后我们可以调用 glBufferData 函数，它会把之前定义的顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // glVertexAttribPointer 函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
    // 这里 0 对应了 layout (location = 0) 中设置的位置 0, 从而使得 layout (location = 0) 这个位置可以与顶点缓冲对象中的数据关联起来
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 然后必须启用顶点属性, 这些属性才生效, 这里启用第0个顶点属性
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    // 解绑 VBO, 请注意，这是允许的，调用 glVertexAttribPointer 会将VBO注册为vertex属性的绑定顶点缓冲区对象，这样之后我们就可以安全地解除绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    // 记住: 当VAO处于活动状态时，不要解除绑定EBO，因为绑定的元素缓冲区对象存储在VAO中；需保持EBO绑定
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // 您可以在之后解绑 VAO，这样其他 VAO 调用就不会意外地修改该VAO，但这种情况很少发生。
    // 修改其他 VAOS 无论如何都需要调用 glBindVertexArray，所以我们通常不会在不直接需要时解除绑定VAOS(或VBO)。
    glBindVertexArray(0); 

    // 取消注释此调用以在线框多边形中绘制
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);

        // 每次调用都确保绑定了 VAO, 因为我们只有一个VAO，所以不需要每次都绑定它，但我们这样做是为了让事情更有条理一些
        glBindVertexArray(VAO);

        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // 不需要每次都解绑
 
        // glfw: 交换缓冲区和轮询IO事件(按下/释放键、移动鼠标等)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 可选：一旦所有资源的使用寿命超过其用途，请取消分配
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // glfw: 终止，清除所有先前分配的GLFW资源
    glfwTerminate();
    return 0;
}

// glfw 处理所有输入：查询GLFW本帧是否按下/释放了相关按键，并做出相应反应
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: 每当窗口大小更改(通过操作系统或用户调整大小)时，此回调函数将执行
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // 确保视区与新的窗口尺寸匹配；请注意，宽度和高度将明显大于视网膜显示器上指定的尺寸。
    glViewport(0, 0, width, height);
}
