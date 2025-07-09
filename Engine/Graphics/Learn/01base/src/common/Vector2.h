#ifndef __VECTOR2_H__
#define __VECTOR2_H__

class Vector2
{
  public:
    float x, y;

    Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y)
    {
    }

    // 向量减法
    Vector2 operator-(const Vector2 &other) const
    {
        return Vector2(x - other.x, y - other.y);
    }

    // 向量叉乘
    float cross(const Vector2 &other) const
    {
        return x * other.y - y * other.x;
    }
};


#endif
