#include <gtest/gtest.h>

#include "geometry3D.hh"

TEST(PlaneLineCross, Test1)
{
    geometry3D::Plane3D plane{{0, 0, 1}, 0};
    geometry3D::Line3D line{{0, 0, 1}, {1, 1, 0}};

    geometry3D::Point3D point = planeLineIntersect(plane, line);
    geometry3D::Point3D expectedPoint{1, 1, 0};

    ASSERT_EQ(point, expectedPoint);
}