
#include <gtest/gtest.h>
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Math/MathFunctions.h"

TEST(MatrixTest, VerifyIdentityOnCreation)
{
    math::Matrix matrix;

    EXPECT_EQ(1.0f, matrix.data[0]);
    EXPECT_EQ(0.0f, matrix.data[1]);
    EXPECT_EQ(0.0f, matrix.data[2]);
    EXPECT_EQ(0.0f, matrix.data[3]);

    EXPECT_EQ(0.0f, matrix.data[4]);
    EXPECT_EQ(1.0f, matrix.data[5]);
    EXPECT_EQ(0.0f, matrix.data[6]);
    EXPECT_EQ(0.0f, matrix.data[7]);

    EXPECT_EQ(0.0f, matrix.data[8]);
    EXPECT_EQ(0.0f, matrix.data[9]);
    EXPECT_EQ(1.0f, matrix.data[10]);
    EXPECT_EQ(0.0f, matrix.data[11]);

    EXPECT_EQ(0.0f, matrix.data[12]);
    EXPECT_EQ(0.0f, matrix.data[13]);
    EXPECT_EQ(0.0f, matrix.data[14]);
    EXPECT_EQ(1.0f, matrix.data[15]);
}

TEST(MatrixTest, VerifyTranslateFunction)
{
    math::Matrix matrix;
    math::Translate(matrix, math::Vector(10.0f, 22.0f));

    EXPECT_EQ(1.0f, matrix.data[0]);
    EXPECT_EQ(0.0f, matrix.data[1]);
    EXPECT_EQ(0.0f, matrix.data[2]);
    EXPECT_EQ(0.0f, matrix.data[3]);

    EXPECT_EQ(0.0f, matrix.data[4]);
    EXPECT_EQ(1.0f, matrix.data[5]);
    EXPECT_EQ(0.0f, matrix.data[6]);
    EXPECT_EQ(0.0f, matrix.data[7]);

    EXPECT_EQ(0.0f, matrix.data[8]);
    EXPECT_EQ(0.0f, matrix.data[9]);
    EXPECT_EQ(1.0f, matrix.data[10]);
    EXPECT_EQ(0.0f, matrix.data[11]);

    EXPECT_EQ(10.0f, matrix.data[12]);
    EXPECT_EQ(22.0f, matrix.data[13]);
    EXPECT_EQ(0.0f, matrix.data[14]);
    EXPECT_EQ(1.0f, matrix.data[15]);
}

TEST(MatrixTest, VerifyPositionFunction)
{
    math::Matrix matrix;
    math::Translate(matrix, math::Vector(123.0f, 312.0f));

    math::Position(matrix, math::Vector(77.7f, -32.1f));

    EXPECT_EQ(1.0f, matrix.data[0]);
    EXPECT_EQ(0.0f, matrix.data[1]);
    EXPECT_EQ(0.0f, matrix.data[2]);
    EXPECT_EQ(0.0f, matrix.data[3]);

    EXPECT_EQ(0.0f, matrix.data[4]);
    EXPECT_EQ(1.0f, matrix.data[5]);
    EXPECT_EQ(0.0f, matrix.data[6]);
    EXPECT_EQ(0.0f, matrix.data[7]);

    EXPECT_EQ(0.0f, matrix.data[8]);
    EXPECT_EQ(0.0f, matrix.data[9]);
    EXPECT_EQ(1.0f, matrix.data[10]);
    EXPECT_EQ(0.0f, matrix.data[11]);

    EXPECT_FLOAT_EQ(77.7f, matrix.data[12]);
    EXPECT_FLOAT_EQ(-32.1f, matrix.data[13]);
    EXPECT_EQ(0.0f, matrix.data[14]);
    EXPECT_EQ(1.0f, matrix.data[15]);
}

TEST(MatrixTest, VerifyRotationFunction)
{
    const math::Matrix matrix = math::CreateMatrixFromZRotation(math::PI());

    EXPECT_EQ(-1.0f, matrix.data[0]);
    EXPECT_FLOAT_EQ(-8.74228e-08f, matrix.data[1]);
    EXPECT_EQ(0.0f, matrix.data[2]);
    EXPECT_EQ(0.0f, matrix.data[3]);

    EXPECT_FLOAT_EQ(8.74228e-08f, matrix.data[4]);
    EXPECT_EQ(-1.0f, matrix.data[5]);
    EXPECT_EQ(0.0f, matrix.data[6]);
    EXPECT_EQ(0.0f, matrix.data[7]);

    EXPECT_EQ(0.0f, matrix.data[8]);
    EXPECT_EQ(0.0f, matrix.data[9]);
    EXPECT_EQ(1.0f, matrix.data[10]);
    EXPECT_EQ(0.0f, matrix.data[11]);

    EXPECT_EQ(0.0f, matrix.data[12]);
    EXPECT_EQ(0.0f, matrix.data[13]);
    EXPECT_EQ(0.0f, matrix.data[14]);
    EXPECT_EQ(1.0f, matrix.data[15]);
}

TEST(MatrixTest, VerifyInverse)
{
    math::Matrix translation_matrix;
    math::Translate(translation_matrix, math::Vector(10.0f, 7.0f));

    const math::Matrix rotation_matrix = math::CreateMatrixFromZRotation(math::PI());

    const math::Matrix& matrix = translation_matrix * rotation_matrix;
    const math::Matrix& inverse = math::Inverse(matrix);

    constexpr math::Vector point1(0.0f, 0.0f);
    const math::Vector& transformed1 = math::Transformed(matrix, point1);
    const math::Vector& transformed_inverse1 = math::Transformed(inverse, transformed1);

    EXPECT_FLOAT_EQ(transformed1.x, 10.0f);
    EXPECT_FLOAT_EQ(transformed1.y, 7.0f);

    EXPECT_FLOAT_EQ(transformed_inverse1.x, 0.0f);
    EXPECT_FLOAT_EQ(transformed_inverse1.y, 0.0f);
}
