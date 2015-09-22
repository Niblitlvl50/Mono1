//
//  PathTest.c
//  Worksample_2
//
//  Created by Niklas Damberg on 13/11/13.
//
//

#include "gtest/gtest.h"
#include "IPath.h"
#include "PathFactory.h"
#include "Vector2f.h"
#include <vector>

TEST(PathTest, CreatePathAndVerifyLength)
{
    std::vector<math::Vector2f> coords;
    coords.push_back(math::Vector2f(0, 0));
    coords.push_back(math::Vector2f(10, 0));
    coords.push_back(math::Vector2f(10, 10));

    const std::shared_ptr<mono::IPath> path = mono::CreatePath(coords);
    const float length = path->Length();
    EXPECT_EQ(20, length);
}

TEST(PathTest, GetPathAtFullLength)
{
    std::vector<math::Vector2f> coords;
    coords.push_back(math::Vector2f(0, 0));
    coords.push_back(math::Vector2f(10, 0));
    
    std::shared_ptr<mono::IPath> path = mono::CreatePath(coords);
    const math::Vector2f atLength10 = path->GetPositionByLength(10);
    EXPECT_EQ(10, atLength10.x);
    EXPECT_EQ(0, atLength10.y);
}

TEST(PathTest, GetPathAtHalfLength)
{
    std::vector<math::Vector2f> coords;
    coords.push_back(math::Vector2f(0, 0));
    coords.push_back(math::Vector2f(10, 0));
    
    std::shared_ptr<mono::IPath> path = mono::CreatePath(coords);
    const math::Vector2f atLength5 = path->GetPositionByLength(5);
    EXPECT_EQ(5, atLength5.x);
    EXPECT_EQ(0, atLength5.y);
}

TEST(PathTest, GetPathAtZeroLength)
{
    std::vector<math::Vector2f> coords;
    coords.push_back(math::Vector2f(0, 0));
    coords.push_back(math::Vector2f(10, 0));
    
    std::shared_ptr<mono::IPath> path = mono::CreatePath(coords);
    const math::Vector2f atLength0 = path->GetPositionByLength(0);
    EXPECT_EQ(0, atLength0.x);
    EXPECT_EQ(0, atLength0.y);
}

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
    EXPECT_EQ(0, atLength0.x);
    EXPECT_EQ(-5, atLength0.y);
    
    const math::Vector2f atLength2 = path->GetPositionByLength(length);
    EXPECT_EQ(2, atLength2.x);
    EXPECT_EQ(10, atLength2.y);
}

TEST(PathTest, CreatePathAndGetPointsAndVerifyTheSame)
{
    const std::vector<math::Vector2f> coords = { math::Vector2f(0, 0),
                                                 math::Vector2f(10, 10),
                                                 math::Vector2f(-100, -777) };

    const std::shared_ptr<mono::IPath> path = mono::CreatePath(coords);
    EXPECT_EQ(coords, path->GetPathPoints());
}

TEST(PathTest, CreatePathFromFileAndVerifyCoordinates)
{
    const std::vector<math::Vector2f> coords = { math::Vector2f(0, 0),
                                                 math::Vector2f(95.78125, 0),
                                                 math::Vector2f(95.78125, -71.83984375),
                                                 math::Vector2f(0, -71.83984375) };

    const std::shared_ptr<mono::IPath> path = mono::CreatePath("rektangel.path");
    EXPECT_EQ(coords, path->GetPathPoints());
}

TEST(PathTest, NonExistingPathFileShouldThrowExpcetion)
{
    EXPECT_THROW(mono::CreatePath("whatever"), std::runtime_error);
}

