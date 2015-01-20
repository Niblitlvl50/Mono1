//
//  MathTest.cpp
//  Mono1
//
//  Created by Niblit on 2012-08-12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "gtest/gtest.h"
#include "Quad.h"
#include "Vector2f.h"
#include "MathFunctions.h"

TEST(MathTest, QuadZero)
{
    EXPECT_EQ(math::zeroQuad.mA, math::zeroVec);
    EXPECT_EQ(math::zeroQuad.mB, math::zeroVec);
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

TEST(MathTest, Vector2fOperator)
{
    const math::Vector2f first(2.0f, 1.1f);
    const math::Vector2f second(-1.0f, 4.0f);
    
    const math::Vector2f result = first + second;
    EXPECT_FLOAT_EQ(1.0f, result.x);
    EXPECT_FLOAT_EQ(5.1f, result.y);
    
    const math::Vector2f subtractResult = first - second;
    EXPECT_FLOAT_EQ(3.0f, subtractResult.x);
    EXPECT_FLOAT_EQ(-2.9f, subtractResult.y);
    
    const math::Vector2f multiResult = first * 3.0f;
    EXPECT_FLOAT_EQ(6.0f, multiResult.x);
    EXPECT_FLOAT_EQ(3.3f, multiResult.y);
    
    const math::Vector2f divideRestul1 = first / 2.0f;
    EXPECT_FLOAT_EQ(1.0f, divideRestul1.x);
    EXPECT_FLOAT_EQ(0.55f, divideRestul1.y);
    
    const math::Vector2f divideResult2 = first / math::Vector2f(5.0f, 4.0f);
    EXPECT_FLOAT_EQ(0.4f, divideResult2.x);
    EXPECT_FLOAT_EQ(0.275f, divideResult2.y);
    
    math::Vector2f addResult(1.0f, 0.0f);
    addResult += math::Vector2f(7.7f, -3.0f);
    EXPECT_FLOAT_EQ(8.7f, addResult.x);
    EXPECT_FLOAT_EQ(-3.0f, addResult.y);
}

