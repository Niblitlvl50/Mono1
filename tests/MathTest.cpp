
#include "gtest/gtest.h"
#include "Math/Quad.h"
#include "Math/Vector.h"
#include "Math/MathFunctions.h"
#include "Math/Bezier.h"

TEST(MathTest, QuadZero)
{
    EXPECT_EQ(math::ZeroQuad.mA, math::ZeroVec);
    EXPECT_EQ(math::ZeroQuad.mB, math::ZeroVec);
}

TEST(MathTest, QuadOverlap)
{
    math::Quad first(0.0f, 0.0f, 5.0f, 4.0f);
    math::Quad second(5.0f, 0.0f, 6.0f, 1.0f);
    
    const bool result1 = math::QuadOverlaps(first, second);
    const bool reverse1 = math::QuadOverlaps(second, first);
    EXPECT_FALSE(result1);
    EXPECT_FALSE(reverse1);
    
    math::Quad third(1.0f, 1.0f, 3.0f, 3.0f);
    math::Quad fourth(1.5f, 1.9f, 2.5f, 2.9f);
    
    const bool result2 = math::QuadOverlaps(third, fourth);
    const bool reverse2 = math::QuadOverlaps(fourth, third);
    
    EXPECT_TRUE(result2);
    EXPECT_TRUE(reverse2);
    
    math::Quad fifth(1.0f, 1.0f, 3.0f, 3.0f);
    math::Quad sixth(-1.0f, -1.0f, 1.1f, 0.0f);
    
    const bool result3 = math::QuadOverlaps(fifth, sixth);
    const bool reverse3 = math::QuadOverlaps(sixth, fifth);
    
    EXPECT_FALSE(result3);
    EXPECT_FALSE(reverse3);
    
    math::Quad seventh(1.0f, 1.0f, 3.0f, 3.0f);
    math::Quad eigth(-1.0f, -1.0f, 1.1f, 1.1f);
    
    const bool result4 = math::QuadOverlaps(seventh, eigth);
    const bool reverse4 = math::QuadOverlaps(eigth, seventh);
    
    EXPECT_TRUE(result4);
    EXPECT_TRUE(reverse4);
}

TEST(MathTest, QuadOperator)
{
    math::Quad first(-1.0f, -1.0f, 2.0f, 2.0f);
    const math::Quad second(5.0f, 5.0f, 6.0f, 6.0f);
    
    first |= second;
    
    EXPECT_FLOAT_EQ(-1.0f, first.mA.x);
    EXPECT_FLOAT_EQ(-1.0f, first.mA.y);
    EXPECT_FLOAT_EQ(6.0f, first.mB.x);
    EXPECT_FLOAT_EQ(6.0f, first.mB.y);
    
    math::Quad third(0.0f, -1.0f, 5.0f, 6.0f);
    const math::Quad fourth(-2.0f, -3.0f, 10.0f, 11.0f);
    
    third |= fourth;
    
    EXPECT_FLOAT_EQ(-2.0f, third.mA.x);
    EXPECT_FLOAT_EQ(-3.0f, third.mA.y);
    EXPECT_FLOAT_EQ(10.0f, third.mB.x);
    EXPECT_FLOAT_EQ(11.0f, third.mB.y);
    
    math::Quad fifth(-1.0f, -1.0f, 2.0f, 1.0f);
    const math::Quad sixth(1.0f, 1.0f, 2.0f, 2.0f);
    
    fifth |= sixth;
    
    EXPECT_FLOAT_EQ(-1.0f, fifth.mA.x);
    EXPECT_FLOAT_EQ(-1.0f, fifth.mA.y);
    EXPECT_FLOAT_EQ(2.0f, fifth.mB.x);
    EXPECT_FLOAT_EQ(2.0f, fifth.mB.y);
    
    
    const math::Quad seventh(-1.0f, 3.0f, 5.0f, 2.0f);
    const math::Quad result = seventh * 3.0f;
    
    EXPECT_FLOAT_EQ(-3.0f, result.mA.x);
    EXPECT_FLOAT_EQ(9.0f, result.mA.y);
    EXPECT_FLOAT_EQ(15.0f, result.mB.x);
    EXPECT_FLOAT_EQ(6.0f, result.mB.y);
}

TEST(MathTest, QuadOperator_second)
{
    math::Quad first(1.0f, 1.0f, 1.0f, 1.0f);
    math::Quad second(1.0f, 1.0f, 1.0f, 1.0f);
    
    const math::Quad left(-0.5f, 1.0f, 1.0f, 1.0f);
    const math::Quad right(2.5f, 1.0f, 1.0f, 1.0f);
    
    first |= left;
    first |= right;
    
    second |= right;
    second |= left;
    
    EXPECT_FLOAT_EQ(first.mA.x, second.mA.x);
    EXPECT_FLOAT_EQ(first.mA.y, second.mA.y);
    EXPECT_FLOAT_EQ(first.mB.x, second.mB.x);
    EXPECT_FLOAT_EQ(first.mB.y, second.mB.y);
}

TEST(MathTest, VectorOperator)
{
    const math::Vector first(2.0f, 1.1f);
    const math::Vector second(-1.0f, 4.0f);
    
    const math::Vector result = first + second;
    EXPECT_FLOAT_EQ(1.0f, result.x);
    EXPECT_FLOAT_EQ(5.1f, result.y);
    
    const math::Vector subtractResult = first - second;
    EXPECT_FLOAT_EQ(3.0f, subtractResult.x);
    EXPECT_FLOAT_EQ(-2.9f, subtractResult.y);
    
    const math::Vector multiResult = first * 3.0f;
    EXPECT_FLOAT_EQ(6.0f, multiResult.x);
    EXPECT_FLOAT_EQ(3.3f, multiResult.y);
    
    const math::Vector divideRestul1 = first / 2.0f;
    EXPECT_FLOAT_EQ(1.0f, divideRestul1.x);
    EXPECT_FLOAT_EQ(0.55f, divideRestul1.y);
    
    const math::Vector divideResult2 = first / math::Vector(5.0f, 4.0f);
    EXPECT_FLOAT_EQ(0.4f, divideResult2.x);
    EXPECT_FLOAT_EQ(0.275f, divideResult2.y);
    
    math::Vector addResult(1.0f, 0.0f);
    addResult += math::Vector(7.7f, -3.0f);
    EXPECT_FLOAT_EQ(8.7f, addResult.x);
    EXPECT_FLOAT_EQ(-3.0f, addResult.y);
}

TEST(MathTest, Bezier)
{
    math::Vector points[4];
    points[0] = math::Vector(1.0f, 1.0f);
    points[1] = math::Vector(3.0f, 1.0f);
    points[2] = math::Vector(2.0f, -5.0f);
    points[3] = math::Vector(3.14f, 3.14f);

    const math::Vector& start_point = math::Cubic(0.0f, points);
    const math::Vector& end_point = math::Cubic(1.0f, points);

    EXPECT_FLOAT_EQ(1.0f, start_point.x);
    EXPECT_FLOAT_EQ(1.0f, start_point.y);

    EXPECT_FLOAT_EQ(3.14f, end_point.x);
    EXPECT_FLOAT_EQ(3.14f, end_point.y);
}

TEST(MathTest, VectorFromAngle)
{
    const math::Vector& north = math::VectorFromAngle(0.0f);
    EXPECT_FLOAT_EQ(0.0f, north.x);
    EXPECT_FLOAT_EQ(1.0f, north.y);

    const math::Vector& west = math::VectorFromAngle(math::PI_2());
    EXPECT_FLOAT_EQ(-1.0f, west.x);
    EXPECT_NEAR(0.0f, west.y, 0.0000001f);

    const math::Vector& south = math::VectorFromAngle(math::PI());
    EXPECT_NEAR(0.0f, south.x, 0.0000001f);
    EXPECT_FLOAT_EQ(-1.0f, south.y);

    const math::Vector& east = math::VectorFromAngle(-math::PI_2());
    EXPECT_FLOAT_EQ(1.0f, east.x);
    EXPECT_NEAR(0.0f, east.y, 0.0000001f);
}

TEST(MathTest, AngleFromVector)
{
    const float north = math::AngleFromVector(math::Vector(0.0f, 1.0f));
    EXPECT_FLOAT_EQ(0.0f, north);

    const float west = math::AngleFromVector(math::Vector(-1.0f, 0.0f));
    EXPECT_FLOAT_EQ(math::PI_2(), west);

    const float south = math::AngleFromVector(math::Vector(0.0f, -1.0f));
    EXPECT_FLOAT_EQ(math::PI(), std::abs(south));

    const float east = math::AngleFromVector(math::Vector(1.0f, 0.0f));
    EXPECT_FLOAT_EQ(-math::PI_2(), east);
}

TEST(MathTest, Scale01)
{
    const float result1 = math::Scale01(1.0f, 0.0f, 10.0f);
    ASSERT_FLOAT_EQ(0.1f, result1);

    const float result2 = math::Scale01(25, 0, 50);
    ASSERT_FLOAT_EQ(0.5f, result2);

    const float result3 = math::Scale01(-10, -10, 5);
    ASSERT_FLOAT_EQ(0.0f, result3);

    const float result4 = math::Scale01(0.1f, 1.0f, 0.0f);
    ASSERT_FLOAT_EQ(0.9f, result4);

    const float result5 = math::Scale01Clamped(20, 0, 10);
    ASSERT_FLOAT_EQ(1.0f, result5);

    const float result6 = math::Scale01(20, 0, 10);
    ASSERT_FLOAT_EQ(2.0f, result6);

    const float result7 = math::Scale01Clamped(20, 10, 0);
    ASSERT_FLOAT_EQ(0.0f, result7);

    const float result8 = math::Scale01(20, 10, 0);
    ASSERT_FLOAT_EQ(-1.0f, result8);
}
