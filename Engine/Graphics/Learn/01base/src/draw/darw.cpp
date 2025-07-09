#include "common.h"

// 绘制正方形
void drawRectangle(int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    for (int i = x; i < x + width; ++i)
    {
        for (int j = y; j < y + height; ++j)
        {
            drawPixel(i, j, r, g, b, a);
        }
    }
}

// 绘制直线（使用 Bresenham 算法）
void drawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        drawPixel(x1, y1, r, g, b, a);

        if (x1 == x2 && y1 == y2)
        {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

// 绘制二次贝塞尔曲线
void drawBezierCurve(int x1, int y1, int x2, int y2, int x3, int y3, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    const int steps = 100;
    for (int i = 0; i <= steps; ++i)
    {
        float t = static_cast<float>(i) / steps;
        float u = 1 - t;
        float tt = t * t;
        float uu = u * u;

        int x = static_cast<int>(uu * x1 + 2 * u * t * x2 + tt * x3);
        int y = static_cast<int>(uu * y1 + 2 * u * t * y2 + tt * y3);

        drawPixel(x, y, r, g, b, a);
    }
}

// 绘制圆形（使用中点圆算法）
void drawCircle(int centerX, int centerY, int radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (x <= y)
    {
        // 利用圆的对称性绘制 8 个点
        drawPixel(centerX + x, centerY + y, r, g, b, a);
        drawPixel(centerX - x, centerY + y, r, g, b, a);
        drawPixel(centerX + x, centerY - y, r, g, b, a);
        drawPixel(centerX - x, centerY - y, r, g, b, a);
        drawPixel(centerX + y, centerY + x, r, g, b, a);
        drawPixel(centerX - y, centerY + x, r, g, b, a);
        drawPixel(centerX + y, centerY - x, r, g, b, a);
        drawPixel(centerX - y, centerY - x, r, g, b, a);

        if (d < 0)
        {
            d = d + 4 * x + 6;
        }
        else
        {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

