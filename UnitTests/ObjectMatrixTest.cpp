//
//  ObjectMatrixTest.cpp
//  King_worksample_OSX
//
//  Created by Niklas Damberg on 05/11/13.
//
//

#include "gtest/gtest.h"

#include "ObjectMatrix.h"

TEST(ObjectMatrixTest, TestCreation)
{
    game::ObjectMatrix<int, 4, 4> objects;
    objects.GetRow(0);
}

TEST(ObjectMatrixTest, TestGetRow)
{
    game::ObjectMatrix<int, 4, 4> objects;
    
    objects.Assign(77, 1, 0);
    objects.Assign(77, 1, 1);
    objects.Assign(77, 1, 2);
    objects.Assign(77, 1, 3);
    
    std::array<int, 4> row = objects.GetRow(1);
    for(int value : row)
        EXPECT_EQ(77, value);
}