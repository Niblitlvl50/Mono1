//
//  PathTest.c
//  King_worksample_OSX
//
//  Created by Niklas Damberg on 13/11/13.
//
//

#include "gtest/gtest.h"
#include "IPath.h"
#include "PathFactory.h"
#include "Vector2f.h"
#include <vector>

TEST(PathTest, CreatePath)
{
    std::vector<math::Vector2f> coords;
    coords.push_back(math::Vector2f(0, 0));
    coords.push_back(math::Vector2f(10, 0));

    std::shared_ptr<mono::IPath> path = mono::CreatePath(coords);
    const float length = path->Length();
    EXPECT_EQ(10, length);
}

TEST(PathTest, GetPathAtFullLength)
{
    std::vector<math::Vector2f> coords;
    coords.push_back(math::Vector2f(0, 0));
    coords.push_back(math::Vector2f(10, 0));
    
    std::shared_ptr<mono::IPath> path = mono::CreatePath(coords);
    const math::Vector2f atLength10 = path->GetPositionByLength(10);
    EXPECT_EQ(10, atLength10.mX);
    EXPECT_EQ(0, atLength10.mY);
}

TEST(PathTest, GetPathAtHalfLength)
{
    std::vector<math::Vector2f> coords;
    coords.push_back(math::Vector2f(0, 0));
    coords.push_back(math::Vector2f(10, 0));
    
    std::shared_ptr<mono::IPath> path = mono::CreatePath(coords);
    const math::Vector2f atLength5 = path->GetPositionByLength(5);
    EXPECT_EQ(5, atLength5.mX);
    EXPECT_EQ(0, atLength5.mY);
}

TEST(PathTest, GetPathAtZeroLength)
{
    std::vector<math::Vector2f> coords;
    coords.push_back(math::Vector2f(0, 0));
    coords.push_back(math::Vector2f(10, 0));
    
    std::shared_ptr<mono::IPath> path = mono::CreatePath(coords);
    const math::Vector2f atLength0 = path->GetPositionByLength(0);
    EXPECT_EQ(0, atLength0.mX);
    EXPECT_EQ(0, atLength0.mY);
}

/*
TEST(PathTest, GetPositionFromComplexPath)
{
    std::vector<math::Vector2f> coords;
    coords.push_back(math::Vector2f(0, -5));
    coords.push_back(math::Vector2f(10, 0));
    coords.push_back(math::Vector2f(12, 4));
    coords.push_back(math::Vector2f(2, 10));
    
    std::shared_ptr<mono::IPath> path = mono::CreatePath(coords);
    const float length = path->Length();
    
    const math::Vector2f atLength0 = path->GetPositionByLength(0);
    EXPECT_EQ(0, atLength0.mX);
    EXPECT_EQ(-5, atLength0.mY);
    
    const math::Vector2f atLength2 = path->GetPositionByLength(length);
    EXPECT_EQ(2, atLength2.mX);
    EXPECT_EQ(10, atLength2.mY);
}
 */

