//
//  Matrix.h
//  MonoOSX
//
//  Created by Niklas Damberg on 26/11/14.
//
//

#pragma once

#include "MathFunctions.h"

namespace math
{
    struct Matrix
    {
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
    void Translate(Matrix& matrix, const Vector2f& vector);
    void Position(Matrix& matrix, const Vector2f& position);
    void RotateZ(Matrix& matrix, float radians);
    void ScaleXY(Matrix& matrix, const Vector2f& scale);
    void Transpose(Matrix& matrix);

    void operator *= (Matrix& left, const Matrix& right);

    Matrix Ortho(float left, float right, float bottom, float top, float near, float far);
}
