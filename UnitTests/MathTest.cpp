//
//  MathTest.cpp
//  Mono1
//
//  Created by Niblit on 2012-08-12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "gtest.h"
#include "Quad.h"
#include "Vector2f.h"
#include "MathFunctions.h"

TEST(MathTest, QuadOverlap)
{
    Math::Quad first(0.0f, 0.0f, 5.0f, 4.0f);
    Math::Quad second(5.0f, 0.0f, 6.0f, 1.0f);
    
    const bool result1 = Math::QuadOverlaps(first, second);
    const bool reverse1 = Math::QuadOverlaps(second, first);
    EXPECT_FALSE(result1);
    EXPECT_FALSE(reverse1);
    
    Math::Quad third(1.0f, 1.0f, 3.0f, 3.0f);
    Math::Quad fourth(1.5f, 1.9f, 2.5f, 2.9f);
    
    const bool result2 = Math::QuadOverlaps(third, fourth);
    const bool reverse2 = Math::QuadOverlaps(fourth, third);
    
    EXPECT_TRUE(result2);
    EXPECT_TRUE(reverse2);
    
    Math::Quad fifth(1.0f, 1.0f, 3.0f, 3.0f);
    Math::Quad sixth(-1.0f, -1.0f, 1.1f, 0.0f);
    
    const bool result3 = Math::QuadOverlaps(fifth, sixth);
    const bool reverse3 = Math::QuadOverlaps(sixth, fifth);
    
    EXPECT_FALSE(result3);
    EXPECT_FALSE(reverse3);
    
    Math::Quad seventh(1.0f, 1.0f, 3.0f, 3.0f);
    Math::Quad eigth(-1.0f, -1.0f, 1.1f, 1.1f);
    
    const bool result4 = Math::QuadOverlaps(seventh, eigth);
    const bool reverse4 = Math::QuadOverlaps(eigth, seventh);
    
    EXPECT_TRUE(result4);
    EXPECT_TRUE(reverse4);
}

TEST(MathTest, QuadOperator)
{
    Math::Quad first(-1.0f, -1.0f, 2.0f, 2.0f);
    const Math::Quad second(5.0f, 5.0f, 6.0f, 6.0f);
    
    first |= second;
    
    EXPECT_FLOAT_EQ(-1.0f, first.mA.mX);
    EXPECT_FLOAT_EQ(-1.0f, first.mA.mY);
    EXPECT_FLOAT_EQ(6.0f, first.mB.mX);
    EXPECT_FLOAT_EQ(6.0f, first.mB.mY);
    
    Math::Quad third(0.0f, -1.0f, 5.0f, 6.0f);
    const Math::Quad fourth(-2.0f, -3.0f, 10.0f, 11.0f);
    
    third |= fourth;
    
    EXPECT_FLOAT_EQ(-2.0f, third.mA.mX);
    EXPECT_FLOAT_EQ(-3.0f, third.mA.mY);
    EXPECT_FLOAT_EQ(10.0f, third.mB.mX);
    EXPECT_FLOAT_EQ(11.0f, third.mB.mY);
    
    Math::Quad fifth(-1.0f, -1.0f, 2.0f, 1.0f);
    const Math::Quad sixth(1.0f, 1.0f, 2.0f, 2.0f);
    
    fifth |= sixth;
    
    EXPECT_FLOAT_EQ(-1.0f, fifth.mA.mX);
    EXPECT_FLOAT_EQ(-1.0f, fifth.mA.mY);
    EXPECT_FLOAT_EQ(2.0f, fifth.mB.mX);
    EXPECT_FLOAT_EQ(2.0f, fifth.mB.mY);
    
    
    const Math::Quad seventh(-1.0f, 3.0f, 5.0f, 2.0f);
    const Math::Quad result = seventh * 3.0f;
    
    EXPECT_FLOAT_EQ(-3.0f, result.mA.mX);
    EXPECT_FLOAT_EQ(9.0f, result.mA.mY);
    EXPECT_FLOAT_EQ(15.0f, result.mB.mX);
    EXPECT_FLOAT_EQ(6.0f, result.mB.mY);
}

TEST(MathTest, QuadOperator_second)
{
    Math::Quad first(1.0f, 1.0f, 1.0f, 1.0f);
    Math::Quad second(1.0f, 1.0f, 1.0f, 1.0f);
    
    const Math::Quad left(-0.5f, 1.0f, 1.0f, 1.0f);
    const Math::Quad right(2.5f, 1.0f, 1.0f, 1.0f);
    
    first |= left;
    first |= right;
    
    second |= right;
    second |= left;
    
    EXPECT_FLOAT_EQ(first.mA.mX, second.mA.mX);
    EXPECT_FLOAT_EQ(first.mA.mY, second.mA.mY);
    EXPECT_FLOAT_EQ(first.mB.mX, second.mB.mX);
    EXPECT_FLOAT_EQ(first.mB.mY, second.mB.mY);
}

TEST(MathTest, Vector2fOperator)
{
    const Math::Vector2f first(2.0f, 1.1f);
    const Math::Vector2f second(-1.0f, 4.0f);
    
    const Math::Vector2f result = first + second;
    EXPECT_FLOAT_EQ(1.0f, result.mX);
    EXPECT_FLOAT_EQ(5.1f, result.mY);
    
    const Math::Vector2f subtractResult = first - second;
    EXPECT_FLOAT_EQ(3.0f, subtractResult.mX);
    EXPECT_FLOAT_EQ(-2.9f, subtractResult.mY);
    
    const Math::Vector2f multiResult = first * 3.0f;
    EXPECT_FLOAT_EQ(6.0f, multiResult.mX);
    EXPECT_FLOAT_EQ(3.3f, multiResult.mY);
    
    const Math::Vector2f divideRestul1 = first / 2.0f;
    EXPECT_FLOAT_EQ(1.0f, divideRestul1.mX);
    EXPECT_FLOAT_EQ(0.55f, divideRestul1.mY);
    
    const Math::Vector2f divideResult2 = first / Math::Vector2f(5.0f, 4.0f);
    EXPECT_FLOAT_EQ(0.4f, divideResult2.mX);
    EXPECT_FLOAT_EQ(0.275f, divideResult2.mY);
    
    Math::Vector2f addResult(1.0f, 0.0f);
    addResult += Math::Vector2f(7.7f, -3.0f);
    EXPECT_FLOAT_EQ(8.7f, addResult.mX);
    EXPECT_FLOAT_EQ(-3.0f, addResult.mY);
}

