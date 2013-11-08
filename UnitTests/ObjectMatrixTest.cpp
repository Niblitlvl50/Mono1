//
//  ObjectMatrixTest.cpp
//  King_worksample_OSX
//
//  Created by Niklas Damberg on 05/11/13.
//
//

#include "gtest/gtest.h"
#include "ObjectMatrix.h"

namespace testHelper
{
    template <typename T, typename V>
    void AssignMatrix(T& matrix, V value)
    {
        for(int row = 0; row < matrix.Rows(); ++row)
        {
            for(int column = 0; column < matrix.Columns(); ++column)
                matrix.Assign(value, row, column);
        }
    }
}

TEST(ObjectMatrixTest, TestCreation)
{
    game::ObjectMatrix<int, 4, 5> objects;
    EXPECT_EQ(4, objects.Rows());
    EXPECT_EQ(5, objects.Columns());
}

TEST(ObjectMatrixTest, TestGetRow)
{
    game::ObjectMatrix<int, 4, 4> objects;
    testHelper::AssignMatrix(objects, 0);
    
    objects.Assign(11, 1, 0);
    objects.Assign(22, 1, 1);
    objects.Assign(33, 1, 2);
    objects.Assign(44, 1, 3);
  
    auto it = objects.iterate_row(1);
    EXPECT_EQ(11, *it);
    
    ++it;
    EXPECT_EQ(22, *it);
    
    ++it;
    EXPECT_EQ(33, *it);
    
    ++it;
    EXPECT_EQ(44, *it);
}

TEST(ObjectMatrixTest, TestGetColumn)
{
    game::ObjectMatrix<int, 4, 4> objects;
    testHelper::AssignMatrix(objects, 0);
    
    objects.Assign(11, 0, 3);
    objects.Assign(22, 1, 3);
    objects.Assign(33, 2, 3);
    objects.Assign(44, 3, 3);
    
    auto it = objects.iterate_column(3);
    EXPECT_EQ(11, *it);

    ++it;
    EXPECT_EQ(22, *it);

    ++it;
    EXPECT_EQ(33, *it);

    ++it;
    EXPECT_EQ(44, *it);
}

TEST(ObjectMatrixTest, TestIterateRow)
{
    game::ObjectMatrix<int, 3, 4> objects;
    testHelper::AssignMatrix(objects, 0);
    
    int counter = 0;
    auto row = objects.iterate_row(0);
    while(row.HasMore())
    {
        counter++;
        ++row;
    }
    
    EXPECT_EQ(4, counter);
}

