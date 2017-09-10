
#pragma once

#include "MathFwd.h"

namespace math
{
    struct Matrix
    {
        constexpr Matrix()
        { }
        
        //
        // Stored column-major
        //
        // |  0  4  8 12 |
        // |  1  5  9 13 |
        // |  2  6 10 14 |
        // |  3  7 11 15 |
        //
        float data[16] = { 1, 0, 0, 0,
                           0, 1, 0, 0,
                           0, 0, 1, 0,
                           0, 0, 0, 1 };
    };

    void Identity(Matrix& matrix);
    void Translate(Matrix& matrix, const Vector& vector);
    void Position(Matrix& matrix, const Vector& position);
    void RotateZ(Matrix& matrix, float radians);
    void RotateZ(Matrix& matrix, float radians, const Vector& offset);
    void ScaleXY(Matrix& matrix, const Vector& scale);
    void Transpose(Matrix& matrix);

    void Inverse(Matrix& matrix);
    math::Matrix Inverse(const Matrix& matrix);

    void Transform(const Matrix& matrix, math::Vector& vector);
    void Transform(const Matrix& matrix, math::Quad& quad);
    math::Vector Transform(const Matrix& matrix, const math::Vector& vector);
    math::Quad Transform(const Matrix& matrix, const math::Quad& quad);

    void operator *= (Matrix& left, const Matrix& right);
    math::Matrix operator * (const math::Matrix& left, const math::Matrix& right);

    Matrix Ortho(float left, float right, float bottom, float top, float near, float far);
}
