//
//  MatrixTest.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 26/11/14.
//
//

#include <gtest/gtest.h>
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Math/MathFunctions.h"

TEST(MatrixTest, VerifyIdentityOnCreation)
{
    math::Matrix matrix;

    EXPECT_EQ(1, matrix.data[0]);
    EXPECT_EQ(0, matrix.data[1]);
    EXPECT_EQ(0, matrix.data[2]);
    EXPECT_EQ(0, matrix.data[3]);

    EXPECT_EQ(0, matrix.data[4]);
    EXPECT_EQ(1, matrix.data[5]);
    EXPECT_EQ(0, matrix.data[6]);
    EXPECT_EQ(0, matrix.data[7]);

    EXPECT_EQ(0, matrix.data[8]);
    EXPECT_EQ(0, matrix.data[9]);
    EXPECT_EQ(1, matrix.data[10]);
    EXPECT_EQ(0, matrix.data[11]);

    EXPECT_EQ(0, matrix.data[12]);
    EXPECT_EQ(0, matrix.data[13]);
    EXPECT_EQ(0, matrix.data[14]);
    EXPECT_EQ(1, matrix.data[15]);
}

TEST(MatrixTest, VerifyTranslateFunction)
{
    math::Matrix matrix;
    math::Translate(matrix, math::Vector(10, 22));

    EXPECT_EQ(1, matrix.data[0]);
    EXPECT_EQ(0, matrix.data[1]);
    EXPECT_EQ(0, matrix.data[2]);
    EXPECT_EQ(0, matrix.data[3]);

    EXPECT_EQ(0, matrix.data[4]);
    EXPECT_EQ(1, matrix.data[5]);
    EXPECT_EQ(0, matrix.data[6]);
    EXPECT_EQ(0, matrix.data[7]);

    EXPECT_EQ(0, matrix.data[8]);
    EXPECT_EQ(0, matrix.data[9]);
    EXPECT_EQ(1, matrix.data[10]);
    EXPECT_EQ(0, matrix.data[11]);

    EXPECT_EQ(10, matrix.data[12]);
    EXPECT_EQ(22, matrix.data[13]);
    EXPECT_EQ(0, matrix.data[14]);
    EXPECT_EQ(1, matrix.data[15]);
}

TEST(MatrixTest, VerifyPositionFunction)
{
    math::Matrix matrix;
    math::Translate(matrix, math::Vector(123, 312));

    math::Position(matrix, math::Vector(77.7, -32.1));

    EXPECT_EQ(1, matrix.data[0]);
    EXPECT_EQ(0, matrix.data[1]);
    EXPECT_EQ(0, matrix.data[2]);
    EXPECT_EQ(0, matrix.data[3]);

    EXPECT_EQ(0, matrix.data[4]);
    EXPECT_EQ(1, matrix.data[5]);
    EXPECT_EQ(0, matrix.data[6]);
    EXPECT_EQ(0, matrix.data[7]);

    EXPECT_EQ(0, matrix.data[8]);
    EXPECT_EQ(0, matrix.data[9]);
    EXPECT_EQ(1, matrix.data[10]);
    EXPECT_EQ(0, matrix.data[11]);

    EXPECT_FLOAT_EQ(77.7, matrix.data[12]);
    EXPECT_FLOAT_EQ(-32.1, matrix.data[13]);
    EXPECT_EQ(0, matrix.data[14]);
    EXPECT_EQ(1, matrix.data[15]);
}

TEST(MatrixTest, VerifyRotationFunction)
{
    math::Matrix matrix;
    math::RotateZ(matrix, math::PI());

    EXPECT_EQ(-1, matrix.data[0]);
    EXPECT_FLOAT_EQ(-8.74228e-08, matrix.data[1]);
    EXPECT_EQ(0, matrix.data[2]);
    EXPECT_EQ(0, matrix.data[3]);

    EXPECT_FLOAT_EQ(8.74228e-08, matrix.data[4]);
    EXPECT_EQ(-1, matrix.data[5]);
    EXPECT_EQ(0, matrix.data[6]);
    EXPECT_EQ(0, matrix.data[7]);

    EXPECT_EQ(0, matrix.data[8]);
    EXPECT_EQ(0, matrix.data[9]);
    EXPECT_EQ(1, matrix.data[10]);
    EXPECT_EQ(0, matrix.data[11]);

    EXPECT_EQ(0, matrix.data[12]);
    EXPECT_EQ(0, matrix.data[13]);
    EXPECT_EQ(0, matrix.data[14]);
    EXPECT_EQ(1, matrix.data[15]);
}



