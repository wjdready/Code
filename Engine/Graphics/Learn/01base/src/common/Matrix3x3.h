#ifndef __MATRIX3X3_H__
#define __MATRIX3X3_H__

#include "Vector2.h"
#include <corecrt_math.h>

class Matrix3x3
{
  public:
    float data[3][3];

    Matrix3x3()
    {
        // 初始化为单位矩阵
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                data[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }

    // 矩阵乘法
    Matrix3x3 operator*(const Matrix3x3 &other) const
    {
        Matrix3x3 result;
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                result.data[i][j] = 0.0f;
                for (int k = 0; k < 3; ++k)
                {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    // 向量变换
    Vector2 transform(const Vector2 &vec) const
    {
        float newX = data[0][0] * vec.x + data[0][1] * vec.y + data[0][2];
        float newY = data[1][0] * vec.x + data[1][1] * vec.y + data[1][2];
        return Vector2(newX, newY);
    }

    // 创建平移矩阵
    static Matrix3x3 createTranslation(float tx, float ty)
    {
        Matrix3x3 mat;
        mat.data[0][2] = tx;
        mat.data[1][2] = ty;
        return mat;
    }

    // 创建旋转矩阵（弧度制）
    static Matrix3x3 createRotation(float angle)
    {
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
    static Matrix3x3 createScaling(float sx, float sy)
    {
        Matrix3x3 mat;
        mat.data[0][0] = sx;
        mat.data[1][1] = sy;
        return mat;
    }
};


#endif