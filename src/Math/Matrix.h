
#pragma once

#include "MathFwd.h"

namespace math
{
    struct Matrix
    {
        constexpr Matrix()
        { }
        
        //
        // Stored column-major, index inside []
        //
        // |  0[0]  4[1]   8[2]  12[3]  |
        // |  1[4]  5[5]   9[6]  13[7]  |
        // |  2[8]  6[9]  10[10] 14[11] |
        // |  3[12] 7[13] 11[14] 15[15] |
        //
        // https://github.com/toji/gl-matrix is used as inspiration.
        //
        float data[16] = { 1, 0, 0, 0,
                           0, 1, 0, 0,
                           0, 0, 1, 0,
                           0, 0, 0, 1 };
    };

    Matrix CreateMatrixWithPosition(const math::Vector& position);
    Matrix CreateMatrixWithScale(const math::Vector& scale);
    Matrix CreateMatrixFromZRotation(float radians);

    void Identity(Matrix& matrix);
    void Translate(Matrix& matrix, const Vector& vector);
    void Position(Matrix& matrix, const Vector& position);
    math::Vector GetPosition(const Matrix& matrix);
    void RotateX(Matrix& matrix, float radians);
    void RotateZ(Matrix& matrix, float radians);
    void RotateZ(Matrix& matrix, float radians, const Vector& offset);
    void ScaleXY(Matrix& matrix, const Vector& scale);
    void Transpose(Matrix& matrix);

    float GetZRotation(const Matrix& matrix);

    void Inverse(Matrix& matrix);
    math::Matrix Inverse(const Matrix& matrix);

    void Transform(const Matrix& matrix, math::Vector& vector);
    void Transform(const Matrix& matrix, math::Quad& quad);
    math::Vector Transform(const Matrix& matrix, const math::Vector& vector);
    math::Quad Transform(const Matrix& matrix, const math::Quad& quad);

    void operator *= (Matrix& left, const Matrix& right);
    math::Matrix operator * (const math::Matrix& left, const math::Matrix& right);

    Matrix Ortho(float left, float right, float bottom, float top, float near, float far);
    Matrix Perspective(float fov, float aspect_ratio, float near, float far);
}
