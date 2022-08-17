
#include "Matrix.h"
#include "MathFunctions.h"
#include "Vector.h"
#include "Quad.h"
#include <cmath>

void math::Identity(Matrix& matrix)
{
    matrix.data[0] = 1;
    matrix.data[1] = 0;
    matrix.data[2] = 0;
    matrix.data[3] = 0;

    matrix.data[4] = 0;
    matrix.data[5] = 1;
    matrix.data[6] = 0;
    matrix.data[7] = 0;

    matrix.data[8] = 0;
    matrix.data[9] = 0;
    matrix.data[10] = 1;
    matrix.data[11] = 0;

    matrix.data[12] = 0;
    matrix.data[13] = 0;
    matrix.data[14] = 0;
    matrix.data[15] = 1;
}

math::Matrix math::CreateMatrixWithPosition(const math::Vector& position)
{
    math::Matrix matrix;
    math::Translate(matrix, position);
    return matrix;
}

math::Matrix math::CreateMatrixWithScale(const math::Vector& scale)
{
    math::Matrix matrix;
    math::ScaleXY(matrix, scale);
    return matrix;
}

math::Matrix math::CreateMatrixWithScale(float x, float y)
{
    return CreateMatrixWithScale(math::Vector(x, y));
}

math::Matrix math::CreateMatrixFromZRotation(float radians)
{
    math::Matrix matrix;

    const float sine = std::sin(radians);
    const float cosine = std::cos(radians);

    matrix.data[0] = cosine;
    matrix.data[1] = sine;
    matrix.data[4] = -sine;
    matrix.data[5] = cosine;

    return matrix;
}

math::Matrix math::CreateMatrixWithPositionRotation(const math::Vector& position, float rotation_radians)
{
    math::Matrix matrix = CreateMatrixFromZRotation(rotation_radians);
    math::Position(matrix, position);
    return matrix;
}

math::Matrix math::CreateMatrixWithPositionRotationScale(const math::Vector& position, float rotation_radians, const math::Vector& scale)
{
    math::Matrix matrix;
    math::Position(matrix, position);
    math::ScaleXY(matrix, scale);
    math::RotateZ(matrix, rotation_radians);

    return matrix;
}

void math::Translate(Matrix& matrix, const Vector& vector)
{
    matrix.data[12] += vector.x;
    matrix.data[13] += vector.y;
    //matrix.data[14] += vector.z;
}

void math::Position(Matrix& matrix, const Vector& position)
{
    matrix.data[12] = position.x;
    matrix.data[13] = position.y;
    //matrix.data[14] += position.z;
}

math::Vector math::GetPosition(const math::Matrix& matrix)
{
    return math::Vector(matrix.data[12], matrix.data[13]);
}

void math::RotateX(math::Matrix& matrix, float radians)
{
    const float sine = std::sin(radians);
    const float cosine = std::cos(radians);

    const float a10 = matrix.data[4];
    const float a11 = matrix.data[5];
    const float a12 = matrix.data[6];
    const float a13 = matrix.data[7];
    const float a20 = matrix.data[8];
    const float a21 = matrix.data[9];
    const float a22 = matrix.data[10];
    const float a23 = matrix.data[11];

    // Perform axis-specific matrix multiplication
    matrix.data[4] = a10 * cosine + a20 * sine;
    matrix.data[5] = a11 * cosine + a21 * sine;
    matrix.data[6] = a12 * cosine + a22 * sine;
    matrix.data[7] = a13 * cosine + a23 * sine;
    matrix.data[8] = a20 * cosine - a10 * sine;
    matrix.data[9] = a21 * cosine - a11 * sine;
    matrix.data[10] = a22 * cosine - a12 * sine;
    matrix.data[11] = a23 * cosine - a13 * sine;
}

void math::RotateZ(math::Matrix& matrix, float radians)
{
    const float sine = std::sin(radians);
    const float cosine = std::cos(radians);

    const float a00 = matrix.data[0];
    const float a01 = matrix.data[1];
    const float a02 = matrix.data[2];
    const float a03 = matrix.data[3];
    const float a10 = matrix.data[4];
    const float a11 = matrix.data[5];
    const float a12 = matrix.data[6];
    const float a13 = matrix.data[7];

    // Perform axis-specific matrix multiplication
    matrix.data[0] = a00 * cosine + a10 * sine;
    matrix.data[1] = a01 * cosine + a11 * sine;
    matrix.data[2] = a02 * cosine + a12 * sine;
    matrix.data[3] = a03 * cosine + a13 * sine;
    matrix.data[4] = a10 * cosine - a00 * sine;
    matrix.data[5] = a11 * cosine - a01 * sine;
    matrix.data[6] = a12 * cosine - a02 * sine;
    matrix.data[7] = a13 * cosine - a03 * sine;
}

void math::RotateZ(math::Matrix& matrix, float radians, const math::Vector& offset)
{
    const float sine = std::sin(radians);
    const float cosine = std::cos(radians);

    const float m0 = matrix.data[0];
    const float m4 = matrix.data[4];
    const float m8 = matrix.data[8];
    const float m12 = matrix.data[12];

    const float m1 = matrix.data[1];
    const float m5 = matrix.data[5];
    const float m9 = matrix.data[9];
    const float m13 = matrix.data[13];

    matrix.data[0] = m0 * cosine + m1 * -sine;
    matrix.data[1] = m0 * sine   + m1 * cosine;

    matrix.data[4] = m4 * cosine + m5 * -sine;
    matrix.data[5] = m4 * sine   + m5 * cosine;

    matrix.data[8] = m8 * cosine + m9 * -sine;
    matrix.data[9] = m8 * sine   + m9 * cosine;

    matrix.data[12] = m12 * cosine + m13 * -sine;
    matrix.data[13] = m12 * sine   + m13 * cosine;
}

void math::ScaleXY(Matrix& matrix, const Vector& scale)
{
    // The elements of the matrix are stored as column major order.
    // |  0  4  8 12 |
    // |  1  5  9 13 |
    // |  2  6 10 14 |
    // |  3  7 11 15 |

    matrix.data[0] *= scale.x;
    matrix.data[1] *= scale.x;
    matrix.data[2] *= scale.x;
    matrix.data[3] *= scale.x;

    matrix.data[4] *= scale.y;
    matrix.data[5] *= scale.y;
    matrix.data[4] *= scale.y;
    matrix.data[7] *= scale.y;

    //matrix.data[2] *= scale.z;
    //matrix.data[6] *= scale.z;
    //matrix.data[10] *= scale.z;
    //matrix.data[14] *= scale.z;
}

void math::Transpose(Matrix& matrix)
{
    std::swap(matrix.data[1], matrix.data[4]);
    std::swap(matrix.data[2], matrix.data[8]);
    std::swap(matrix.data[3], matrix.data[12]);

    std::swap(matrix.data[6], matrix.data[9]);
    std::swap(matrix.data[7], matrix.data[13]);

    std::swap(matrix.data[11], matrix.data[14]);
}

void math::Inverse(math::Matrix& matrix)
{
    math::Matrix inv;

    inv.data[0] =
        matrix.data[5]  * matrix.data[10] * matrix.data[15] -
        matrix.data[5]  * matrix.data[11] * matrix.data[14] -
        matrix.data[9]  * matrix.data[6]  * matrix.data[15] +
        matrix.data[9]  * matrix.data[7]  * matrix.data[14] +
        matrix.data[13] * matrix.data[6]  * matrix.data[11] -
        matrix.data[13] * matrix.data[7]  * matrix.data[10];

    inv.data[4] =
        -matrix.data[4] * matrix.data[10] * matrix.data[15] +
        matrix.data[4]  * matrix.data[11] * matrix.data[14] +
        matrix.data[8]  * matrix.data[6]  * matrix.data[15] -
        matrix.data[8]  * matrix.data[7]  * matrix.data[14] -
        matrix.data[12] * matrix.data[6]  * matrix.data[11] +
        matrix.data[12] * matrix.data[7]  * matrix.data[10];

    inv.data[8] =
        matrix.data[4]  * matrix.data[9] * matrix.data[15] -
        matrix.data[4]  * matrix.data[11] * matrix.data[13] -
        matrix.data[8]  * matrix.data[5] * matrix.data[15] +
        matrix.data[8]  * matrix.data[7] * matrix.data[13] +
        matrix.data[12] * matrix.data[5] * matrix.data[11] -
        matrix.data[12] * matrix.data[7] * matrix.data[9];

    inv.data[12] =
        -matrix.data[4] * matrix.data[9] * matrix.data[14] +
        matrix.data[4]  * matrix.data[10] * matrix.data[13] +
        matrix.data[8]  * matrix.data[5] * matrix.data[14] -
        matrix.data[8]  * matrix.data[6] * matrix.data[13] -
        matrix.data[12] * matrix.data[5] * matrix.data[10] +
        matrix.data[12] * matrix.data[6] * matrix.data[9];

    inv.data[1] =
        -matrix.data[1] * matrix.data[10] * matrix.data[15] +
        matrix.data[1]  * matrix.data[11] * matrix.data[14] +
        matrix.data[9]  * matrix.data[2] * matrix.data[15] -
        matrix.data[9]  * matrix.data[3] * matrix.data[14] -
        matrix.data[13] * matrix.data[2] * matrix.data[11] +
        matrix.data[13] * matrix.data[3] * matrix.data[10];

    inv.data[5] =
        matrix.data[0]  * matrix.data[10] * matrix.data[15] -
        matrix.data[0]  * matrix.data[11] * matrix.data[14] -
        matrix.data[8]  * matrix.data[2] * matrix.data[15] +
        matrix.data[8]  * matrix.data[3] * matrix.data[14] +
        matrix.data[12] * matrix.data[2] * matrix.data[11] -
        matrix.data[12] * matrix.data[3] * matrix.data[10];

    inv.data[9] =
        -matrix.data[0] * matrix.data[9] * matrix.data[15] +
        matrix.data[0]  * matrix.data[11] * matrix.data[13] +
        matrix.data[8]  * matrix.data[1] * matrix.data[15] -
        matrix.data[8]  * matrix.data[3] * matrix.data[13] -
        matrix.data[12] * matrix.data[1] * matrix.data[11] +
        matrix.data[12] * matrix.data[3] * matrix.data[9];

    inv.data[13] =
        matrix.data[0]  * matrix.data[9] * matrix.data[14] -
        matrix.data[0]  * matrix.data[10] * matrix.data[13] -
        matrix.data[8]  * matrix.data[1] * matrix.data[14] +
        matrix.data[8]  * matrix.data[2] * matrix.data[13] +
        matrix.data[12] * matrix.data[1] * matrix.data[10] -
        matrix.data[12] * matrix.data[2] * matrix.data[9];

    inv.data[2] =
        matrix.data[1]  * matrix.data[6] * matrix.data[15] -
        matrix.data[1]  * matrix.data[7] * matrix.data[14] -
        matrix.data[5]  * matrix.data[2] * matrix.data[15] +
        matrix.data[5]  * matrix.data[3] * matrix.data[14] +
        matrix.data[13] * matrix.data[2] * matrix.data[7] -
        matrix.data[13] * matrix.data[3] * matrix.data[6];

    inv.data[6] =
        -matrix.data[0] * matrix.data[6] * matrix.data[15] +
        matrix.data[0]  * matrix.data[7] * matrix.data[14] +
        matrix.data[4]  * matrix.data[2] * matrix.data[15] -
        matrix.data[4]  * matrix.data[3] * matrix.data[14] -
        matrix.data[12] * matrix.data[2] * matrix.data[7] +
        matrix.data[12] * matrix.data[3] * matrix.data[6];

    inv.data[10] =
        matrix.data[0]  * matrix.data[5] * matrix.data[15] -
        matrix.data[0]  * matrix.data[7] * matrix.data[13] -
        matrix.data[4]  * matrix.data[1] * matrix.data[15] +
        matrix.data[4]  * matrix.data[3] * matrix.data[13] +
        matrix.data[12] * matrix.data[1] * matrix.data[7] -
        matrix.data[12] * matrix.data[3] * matrix.data[5];

    inv.data[14] =
        -matrix.data[0] * matrix.data[5] * matrix.data[14] +
        matrix.data[0]  * matrix.data[6] * matrix.data[13] +
        matrix.data[4]  * matrix.data[1] * matrix.data[14] -
        matrix.data[4]  * matrix.data[2] * matrix.data[13] -
        matrix.data[12] * matrix.data[1] * matrix.data[6] +
        matrix.data[12] * matrix.data[2] * matrix.data[5];

    inv.data[3] =
        -matrix.data[1] * matrix.data[6] * matrix.data[11] +
        matrix.data[1] * matrix.data[7] * matrix.data[10] +
        matrix.data[5] * matrix.data[2] * matrix.data[11] -
        matrix.data[5] * matrix.data[3] * matrix.data[10] -
        matrix.data[9] * matrix.data[2] * matrix.data[7] +
        matrix.data[9] * matrix.data[3] * matrix.data[6];

    inv.data[7] =
        matrix.data[0] * matrix.data[6] * matrix.data[11] -
        matrix.data[0] * matrix.data[7] * matrix.data[10] -
        matrix.data[4] * matrix.data[2] * matrix.data[11] +
        matrix.data[4] * matrix.data[3] * matrix.data[10] +
        matrix.data[8] * matrix.data[2] * matrix.data[7] -
        matrix.data[8] * matrix.data[3] * matrix.data[6];

    inv.data[11] =
        -matrix.data[0] * matrix.data[5] * matrix.data[11] +
        matrix.data[0] * matrix.data[7] * matrix.data[9] +
        matrix.data[4] * matrix.data[1] * matrix.data[11] -
        matrix.data[4] * matrix.data[3] * matrix.data[9] -
        matrix.data[8] * matrix.data[1] * matrix.data[7] +
        matrix.data[8] * matrix.data[3] * matrix.data[5];

    inv.data[15] =
        matrix.data[0] * matrix.data[5] * matrix.data[10] -
        matrix.data[0] * matrix.data[6] * matrix.data[9] -
        matrix.data[4] * matrix.data[1] * matrix.data[10] +
        matrix.data[4] * matrix.data[2] * matrix.data[9] +
        matrix.data[8] * matrix.data[1] * matrix.data[6] -
        matrix.data[8] * matrix.data[2] * matrix.data[5];

    float det =
        (matrix.data[0] * inv.data[0]) +
        (matrix.data[1] * inv.data[4]) +
        (matrix.data[2] * inv.data[8]) +
        (matrix.data[3] * inv.data[12]);
    
    if(det == 0.0f)
        return;

    det = 1.0f / det;

    for(int i = 0; i < 16; i++)
        matrix.data[i] = inv.data[i] * det;
}

math::Matrix math::Inverse(const math::Matrix& matrix)
{
    math::Matrix inverse = matrix;
    math::Inverse(inverse);
    return inverse;
}

void math::Transform(const Matrix& matrix, math::Vector& vector)
{
    const float x = vector.x;
    const float y = vector.y;

    vector.x = matrix.data[0] * x + matrix.data[4] * y + matrix.data[12];
    vector.y = matrix.data[1] * x + matrix.data[5] * y + matrix.data[13];
}

void math::Transform(const Matrix& matrix, math::Quad& quad)
{
    math::Vector bottom_left = math::BottomLeft(quad);
    math::Vector bottom_right = math::BottomRight(quad);
    math::Vector top_left = math::TopLeft(quad);
    math::Vector top_right = math::TopRight(quad);

    math::Transform(matrix, bottom_left);
    math::Transform(matrix, bottom_right);
    math::Transform(matrix, top_left);
    math::Transform(matrix, top_right);

    quad = math::Quad(math::INF, math::INF, -math::INF, -math::INF);

    quad |= bottom_left;
    quad |= bottom_right;
    quad |= top_left;
    quad |= top_right;
}

math::Vector math::Transformed(const Matrix& matrix, const math::Vector& vector)
{
    math::Vector out_vector = vector;
    math::Transform(matrix, out_vector);
    return out_vector;
}

math::Quad math::Transformed(const Matrix& matrix, const math::Quad& quad)
{
    math::Quad out_quad = quad;
    math::Transform(matrix, out_quad);
    return out_quad;
}

void math::operator *= (Matrix& left, const Matrix& right)
{
    const float m0 = left.data[0];
    const float m1 = left.data[1];
    const float m2 = left.data[2];
    const float m3 = left.data[3];

    const float m4 = left.data[4];
    const float m5 = left.data[5];
    const float m6 = left.data[6];
    const float m7 = left.data[7];

    const float m8  = left.data[8];
    const float m9  = left.data[9];
    const float m10 = left.data[10];
    const float m11 = left.data[11];

    const float m12 = left.data[12];
    const float m13 = left.data[13];
    const float m14 = left.data[14];
    const float m15 = left.data[15];

    left.data[0] = m0 * right.data[0] + m4 * right.data[1] + m8  * right.data[2] + m12 * right.data[3];
    left.data[1] = m1 * right.data[0] + m5 * right.data[1] + m9  * right.data[2] + m13 * right.data[3];
    left.data[2] = m2 * right.data[0] + m6 * right.data[1] + m10 * right.data[2] + m14 * right.data[3];
    left.data[3] = m3 * right.data[0] + m7 * right.data[1] + m11 * right.data[2] + m15 * right.data[3];

    left.data[4] = m0 * right.data[4] + m4 * right.data[5] + m8  * right.data[6] + m12 * right.data[7];
    left.data[5] = m1 * right.data[4] + m5 * right.data[5] + m9  * right.data[6] + m13 * right.data[7];
    left.data[6] = m2 * right.data[4] + m6 * right.data[5] + m10 * right.data[6] + m14 * right.data[7];
    left.data[7] = m3 * right.data[4] + m7 * right.data[5] + m11 * right.data[6] + m15 * right.data[7];

    left.data[8]  = m0 * right.data[8] + m4 * right.data[9] + m8  * right.data[10] + m12 * right.data[11];
    left.data[9]  = m1 * right.data[8] + m5 * right.data[9] + m9  * right.data[10] + m13 * right.data[11];
    left.data[10] = m2 * right.data[8] + m6 * right.data[9] + m10 * right.data[10] + m14 * right.data[11];
    left.data[11] = m3 * right.data[8] + m7 * right.data[9] + m11 * right.data[10] + m15 * right.data[11];

    left.data[12] = m0 * right.data[12] + m4 * right.data[13] + m8  * right.data[14] + m12 * right.data[15];
    left.data[13] = m1 * right.data[12] + m5 * right.data[13] + m9  * right.data[14] + m13 * right.data[15];
    left.data[14] = m2 * right.data[12] + m6 * right.data[13] + m10 * right.data[14] + m14 * right.data[15];
    left.data[15] = m3 * right.data[12] + m7 * right.data[13] + m11 * right.data[14] + m15 * right.data[15];
}

math::Matrix math::operator * (const math::Matrix& left, const math::Matrix& right)
{
    math::Matrix result = left;
    result *= right;
    return result;
}

math::Matrix math::Ortho(float left, float right, float bottom, float top, float near, float far)
{
    math::Matrix matrix;

    matrix.data[0]  =  2.0f / (right - left);
    matrix.data[5]  =  2.0f / (top - bottom);
    matrix.data[10] = -2.0f / (far - near);
    matrix.data[12] = - (right + left) / (right - left);
    matrix.data[13] = - (top + bottom) / (top - bottom);
    matrix.data[14] = - (far + near) / (far - near);

    return matrix;
}

math::Matrix math::Perspective(float fov, float aspect_ratio, float near, float far)
{
    math::Matrix matrix;
    const float f = 1.0f / tanf(math::ToRadians(fov) / 2.0f);

    matrix.data[0] = f / aspect_ratio;
    matrix.data[5] = f;
    const float nf = 1.0f / (near - far);
    matrix.data[10] = (far + near) * nf;
    matrix.data[11] = -1.0f;
    matrix.data[14] = (2.0f * far * near) * nf;
    matrix.data[15] = 0;

    return matrix;
}

float math::GetZRotation(const math::Matrix& matrix)
{
    const float m21 = matrix.data[9];
    const float m22 = matrix.data[10];
    const float m00 = matrix.data[0];
    const float m10 = matrix.data[4];
    const float m20 = matrix.data[8];
    const float m02 = matrix.data[2];
    const float m01 = matrix.data[1];
    const float m11 = matrix.data[5];
    const float m12 = matrix.data[6];

    const float T1 = std::atan2(m21, m22);
    const float C2 = std::sqrt(m00 * m00 + m10 * m10);
    const float T2 = std::atan2(-m20, C2);
    const float S1 = std::sin(T1);
    const float C1 = std::cos(T1);
    const float T3 = std::atan2(S1 * m02 - C1 * m01, C1 * m11 - S1 * m12);

    const float x = -T1;
    const float y = -T2;
    const float z = -T3;

    (void)x;
    (void)y;

    return z;
}
