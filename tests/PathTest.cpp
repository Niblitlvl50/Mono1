
#include "gtest/gtest.h"
#include "Paths/IPath.h"
#include "Paths/PathFactory.h"
#include "Math/Vector.h"
#include <vector>

TEST(PathTest, CreatePathAndVerifyLength)
{
    std::vector<math::Vector> coords;
    coords.push_back(math::Vector(0, 0));
    coords.push_back(math::Vector(10, 0));
    coords.push_back(math::Vector(10, 10));

    const mono::IPathPtr path = mono::CreatePath(math::ZeroVec, coords);
    const float length = path->Length();
    EXPECT_EQ(20, length);
}

TEST(PathTest, GetPathAtFullLength)
{
    std::vector<math::Vector> coords;
    coords.push_back(math::Vector(0, 0));
    coords.push_back(math::Vector(10, 0));
    
    mono::IPathPtr path = mono::CreatePath(math::ZeroVec, coords);
    const math::Vector atLength10 = path->GetPositionByLength(10);
    EXPECT_EQ(10, atLength10.x);
    EXPECT_EQ(0, atLength10.y);
}

TEST(PathTest, GetPathAtHalfLength)
{
    std::vector<math::Vector> coords;
    coords.push_back(math::Vector(0, 0));
    coords.push_back(math::Vector(10, 0));
    
    mono::IPathPtr path = mono::CreatePath(math::ZeroVec, coords);
    const math::Vector atLength5 = path->GetPositionByLength(5);
    EXPECT_EQ(5, atLength5.x);
    EXPECT_EQ(0, atLength5.y);
}

TEST(PathTest, GetPathAtZeroLength)
{
    std::vector<math::Vector> coords;
    coords.push_back(math::Vector(0, 0));
    coords.push_back(math::Vector(10, 0));
    
    mono::IPathPtr path = mono::CreatePath(math::ZeroVec, coords);
    const math::Vector atLength0 = path->GetPositionByLength(0);
    EXPECT_EQ(0, atLength0.x);
    EXPECT_EQ(0, atLength0.y);
}

TEST(PathTest, GetPositionFromComplexPath)
{
    std::vector<math::Vector> coords;
    coords.push_back(math::Vector(0, -5));
    coords.push_back(math::Vector(10, 0));
    coords.push_back(math::Vector(12, 4));
    coords.push_back(math::Vector(2, 10));
    
    mono::IPathPtr path = mono::CreatePath(math::ZeroVec, coords);
    const float length = path->Length();
    
    const math::Vector atLength0 = path->GetPositionByLength(0);
    EXPECT_EQ(0, atLength0.x);
    EXPECT_EQ(-5, atLength0.y);
    
    const math::Vector atLength2 = path->GetPositionByLength(length);
    EXPECT_EQ(2, atLength2.x);
    EXPECT_EQ(10, atLength2.y);
}

TEST(PathTest, CreatePathAndGetPointsAndVerifyTheSame)
{
    const std::vector<math::Vector> coords = { math::Vector(0, 0),
                                                 math::Vector(10, 10),
                                                 math::Vector(-100, -777) };

    const mono::IPathPtr path = mono::CreatePath(math::ZeroVec, coords);
    EXPECT_EQ(coords, path->GetPathPoints());
}

TEST(PathTest, CreatePathFromFileAndVerifyCoordinates)
{
    const std::vector<math::Vector> coords = { math::Vector(0, 0),
                                                 math::Vector(95.78125, 0),
                                                 math::Vector(95.78125, -71.83984375),
                                                 math::Vector(0, -71.83984375) };

    const mono::IPathPtr path = mono::CreatePath("tests/TestFiles/rektangel.path");
    EXPECT_EQ(coords, path->GetPathPoints());
}

TEST(PathTest, NonExistingPathFileShouldReturnNullptr)
{
    EXPECT_EQ(mono::CreatePath("whatever"), nullptr);
}

