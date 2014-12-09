//
//  MatrixTest.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 26/11/14.
//
//

#include <gtest/gtest.h>
#include "Matrix.h"
#include "Vector2f.h"

TEST(MatrixTest, VerifyIdentityOnCreation)
{
    const math::Matrix matrix;

    EXPECT_EQ(1, matrix.m00);
    EXPECT_EQ(0, matrix.m01);
    EXPECT_EQ(0, matrix.m02);
    EXPECT_EQ(0, matrix.m03);

    EXPECT_EQ(0, matrix.m10);
    EXPECT_EQ(1, matrix.m11);
    EXPECT_EQ(0, matrix.m12);
    EXPECT_EQ(0, matrix.m13);

    EXPECT_EQ(0, matrix.m20);
    EXPECT_EQ(0, matrix.m21);
    EXPECT_EQ(1, matrix.m22);
    EXPECT_EQ(0, matrix.m23);

    EXPECT_EQ(0, matrix.m30);
    EXPECT_EQ(0, matrix.m31);
    EXPECT_EQ(0, matrix.m32);
    EXPECT_EQ(1, matrix.m33);
}

TEST(MatrixTest, VerifyTranslateFunction)
{
    math::Matrix matrix;
    math::Translate(matrix, math::Vector2f(10, 22));

    EXPECT_EQ(1, matrix.m00);
    EXPECT_EQ(0, matrix.m01);
    EXPECT_EQ(0, matrix.m02);
    EXPECT_EQ(10, matrix.m03);

    EXPECT_EQ(0, matrix.m10);
    EXPECT_EQ(1, matrix.m11);
    EXPECT_EQ(0, matrix.m12);
    EXPECT_EQ(22, matrix.m13);

    EXPECT_EQ(0, matrix.m20);
    EXPECT_EQ(0, matrix.m21);
    EXPECT_EQ(1, matrix.m22);
    EXPECT_EQ(0, matrix.m23);

    EXPECT_EQ(0, matrix.m30);
    EXPECT_EQ(0, matrix.m31);
    EXPECT_EQ(0, matrix.m32);
    EXPECT_EQ(1, matrix.m33);
}

TEST(MatrixTest, VerifyPositionFunction)
{
    math::Matrix matrix;
    math::Translate(matrix, math::Vector2f(123, 312));

    math::Position(matrix, math::Vector2f(77.7, -32.1));

    EXPECT_EQ(1, matrix.m00);
    EXPECT_EQ(0, matrix.m01);
    EXPECT_EQ(0, matrix.m02);
    EXPECT_FLOAT_EQ(77.7, matrix.m03);

    EXPECT_EQ(0, matrix.m10);
    EXPECT_EQ(1, matrix.m11);
    EXPECT_EQ(0, matrix.m12);
    EXPECT_FLOAT_EQ(-32.1, matrix.m13);

    EXPECT_EQ(0, matrix.m20);
    EXPECT_EQ(0, matrix.m21);
    EXPECT_EQ(1, matrix.m22);
    EXPECT_EQ(0, matrix.m23);

    EXPECT_EQ(0, matrix.m30);
    EXPECT_EQ(0, matrix.m31);
    EXPECT_EQ(0, matrix.m32);
    EXPECT_EQ(1, matrix.m33);
}

TEST(MatrixTest, VerifyRotationFunction)
{
    math::Matrix matrix;
    math::RotateZ(matrix, math::PI());

    EXPECT_EQ(-1, matrix.m00);
    EXPECT_FLOAT_EQ(8.74228e-08, matrix.m01);
    EXPECT_EQ(0, matrix.m02);
    EXPECT_EQ(0, matrix.m03);

    EXPECT_FLOAT_EQ(-8.74228e-08, matrix.m10);
    EXPECT_EQ(-1, matrix.m11);
    EXPECT_EQ(0, matrix.m12);
    EXPECT_EQ(0, matrix.m13);

    EXPECT_EQ(0, matrix.m20);
    EXPECT_EQ(0, matrix.m21);
    EXPECT_EQ(1, matrix.m22);
    EXPECT_EQ(0, matrix.m23);

    EXPECT_EQ(0, matrix.m30);
    EXPECT_EQ(0, matrix.m31);
    EXPECT_EQ(0, matrix.m32);
    EXPECT_EQ(1, matrix.m33);
}



