// #define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include <vector>

// 向量类
class Vector2 {
public:
    float x, y;

    Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    // 向量减法
    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }
};

// 3x3 矩阵类
class Matrix3x3 {
public:
    float data[3][3];

    Matrix3x3() {
        // 初始化为单位矩阵
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                data[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }

    // 矩阵乘法
    Matrix3x3 operator*(const Matrix3x3& other) const {
        Matrix3x3 result;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result.data[i][j] = 0.0f;
                for (int k = 0; k < 3; ++k) {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    // 向量变换
    Vector2 transform(const Vector2& vec) const {
        float newX = data[0][0] * vec.x + data[0][1] * vec.y + data[0][2];
        float newY = data[1][0] * vec.x + data[1][1] * vec.y + data[1][2];
        return Vector2(newX, newY);
    }

    // 创建平移矩阵
    static Matrix3x3 createTranslation(float tx, float ty) {
        Matrix3x3 mat;
        mat.data[0][2] = tx;
        mat.data[1][2] = ty;
        return mat;
    }

    // 创建旋转矩阵（弧度制）
    static Matrix3x3 createRotation(float angle) {
        Matrix3x3 mat;
        float cosA = cos(angle);
        float sinA = sin(angle);
        mat.data[0][0] = cosA;
        mat.data[0][1] = -sinA;
        mat.data[1][0] = sinA;
        mat.data[1][1] = cosA;
        return mat;
    }

    // 创建缩放矩阵
    static Matrix3x3 createScaling(float sx, float sy) {
        Matrix3x3 mat;
        mat.data[0][0] = sx;
        mat.data[1][1] = sy;
        return mat;
    }
};

// 自定义画点函数
void drawPixel(SDL_Renderer* renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawPoint(renderer, x, y);
}

// 绘制矩形
void drawRectangle(SDL_Renderer* renderer, int x, int y, int width, int height, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    for (int i = x; i < x + width; ++i) {
        for (int j = y; j < y + height; ++j) {
            drawPixel(renderer, i, j, r, g, b, a);
        }
    }
}

// 绘制圆形（使用中点圆算法）
void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (x <= y) {
        // 利用圆的对称性绘制 8 个点
        drawPixel(renderer, centerX + x, centerY + y, r, g, b, a);
        drawPixel(renderer, centerX - x, centerY + y, r, g, b, a);
        drawPixel(renderer, centerX + x, centerY - y, r, g, b, a);
        drawPixel(renderer, centerX - x, centerY - y, r, g, b, a);
        drawPixel(renderer, centerX + y, centerY + x, r, g, b, a);
        drawPixel(renderer, centerX - y, centerY + x, r, g, b, a);
        drawPixel(renderer, centerX + y, centerY - x, r, g, b, a);
        drawPixel(renderer, centerX - y, centerY - x, r, g, b, a);

        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

// 绘制直线（使用 Bresenham 算法）
void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        drawPixel(renderer, x1, y1, r, g, b, a);

        if (x1 == x2 && y1 == y2) {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// 绘制二次贝塞尔曲线
void drawBezierCurve(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    const int steps = 100;
    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / steps;
        float u = 1 - t;
        float tt = t * t;
        float uu = u * u;

        int x = static_cast<int>(uu * x1 + 2 * u * t * x2 + tt * x3);
        int y = static_cast<int>(uu * y1 + 2 * u * t * y2 + tt * y3);

        drawPixel(renderer, x, y, r, g, b, a);
    }
}

// 计算向量叉积
float crossProduct(const Vector2& a, const Vector2& b) {
    return a.x * b.y - a.y * b.x;
}

// 光栅化三角形
void rasterizeTriangle(SDL_Renderer* renderer, const Vector2& v0, const Vector2& v1, const Vector2& v2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, const Matrix3x3& transform) {
    Vector2 tv0 = transform.transform(v0);
    Vector2 tv1 = transform.transform(v1);
    Vector2 tv2 = transform.transform(v2);

    int minX = static_cast<int>(std::min({tv0.x, tv1.x, tv2.x}));
    int maxX = static_cast<int>(std::max({tv0.x, tv1.x, tv2.x}));
    int minY = static_cast<int>(std::min({tv0.y, tv1.y, tv2.y}));
    int maxY = static_cast<int>(std::max({tv0.y, tv1.y, tv2.y}));

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            Vector2 p(x + 0.5f, y + 0.5f);
            Vector2 edge0 = tv1 - tv0;
            Vector2 edge1 = tv2 - tv1;
            Vector2 edge2 = tv0 - tv2;
            Vector2 p0 = p - tv0;
            Vector2 p1 = p - tv1;
            Vector2 p2 = p - tv2;

            float c0 = crossProduct(edge0, p0);
            float c1 = crossProduct(edge1, p1);
            float c2 = crossProduct(edge2, p2);

            if ((c0 >= 0 && c1 >= 0 && c2 >= 0) || (c0 < 0 && c1 < 0 && c2 < 0)) {
                drawPixel(renderer, x, y, r, g, b, a);
            }
        }
    }
}

// 判断点是否在多边形内（射线法）
bool isPointInPolygon(const Vector2& point, const std::vector<Vector2>& polygon) {
    bool inside = false;
    int j = polygon.size() - 1;
    for (size_t i = 0; i < polygon.size(); ++j = i++) {
        if (((polygon[i].y > point.y) != (polygon[j].y > point.y)) &&
            (point.x < (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}

int main(int argc, char* argv[]) {
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 创建窗口
    SDL_Window* window = SDL_CreateWindow("SDL Drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 清屏
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 绘制矩形
    drawRectangle(renderer, 100, 100, 200, 150, 255, 0, 0, 255);

    // 绘制圆形
    drawCircle(renderer, 400, 300, 50, 0, 255, 0, 255);

    // 绘制直线
    drawLine(renderer, 50, 50, 750, 550, 0, 0, 255, 255);

    // 绘制二次贝塞尔曲线
    drawBezierCurve(renderer, 100, 500, 400, 100, 700, 500, 255, 255, 0, 255);

    // 图形变换示例
    Vector2 v0(200, 400);
    Vector2 v1(300, 500);
    Vector2 v2(400, 400);

    // 创建变换矩阵：先平移 (100, 50)，再旋转 45 度（转换为弧度），最后缩放 (1.5, 1.5)
    Matrix3x3 translation = Matrix3x3::createTranslation(500, -200);
    Matrix3x3 rotation = Matrix3x3::createRotation(30.0f * M_PI / 180.0f);
    Matrix3x3 scaling = Matrix3x3::createScaling(2.0f, 2.0f);
    Matrix3x3 transform = translation * rotation * scaling;

    // 光栅化变换后的三角形
    rasterizeTriangle(renderer, v0, v1, v2, 255, 165, 0, 255, transform);

    // 更新屏幕
    SDL_RenderPresent(renderer);

    // 等待用户关闭窗口
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    // 清理资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}