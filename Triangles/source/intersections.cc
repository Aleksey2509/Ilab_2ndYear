#include "geometry3D.hh"

namespace geometry3D
{

// needs explanation comment
Point3D lineLineIntersect(const Line3D& lhs, const Line3D& rhs)
{
    if (!lhs.valid() || !rhs.valid())
        return Point3D{};

    if (lhs.collinearToVector(rhs.direction))
        return Point3D{};


    Vector3D startingPointsDifference{lhs.point, rhs.point};
    if (!floatZero(startingPointsDifference.scalarProduct(lhs.direction.crossProduct(rhs.direction))))
        return Point3D{};

    float_t det = lhs.direction.coords[Y] * rhs.direction.coords[X] - lhs.direction.coords[X] * rhs.direction.coords[Y];

    float_t detT1 = lhs.direction.coords[Y] * (rhs.point.coords[X] - lhs.point.coords[X]) -
                    lhs.direction.coords[X] * (rhs.point.coords[Y] - lhs.point.coords[Y]);

    float_t t1 = detT1 / det;
    Vector3D r0{lhs.point.coords[X], lhs.point.coords[Y], lhs.point.coords[Z]};

    Vector3D radiusVectorOfIntersec{r0 + lhs.direction * t1};

    return Point3D{radiusVectorOfIntersec.coords[X], radiusVectorOfIntersec.coords[Y], radiusVectorOfIntersec.coords[Z]};
}

// needs explanation comment
Point3D planeLineIntersect(const Plane3D& plane, const Line3D& line)
{
    if (!plane.valid() || !line.valid())
        return Point3D{};

    if (floatZero(plane.norm.scalarProduct(line.direction)))
        return Point3D{};

    Vector3D r0{line.point.coords[X], line.point.coords[Y], line.point.coords[Z]};

    float_t t = (plane.norm.scalarProduct(r0) + plane.d) / (plane.norm.scalarProduct(line.direction));

    Vector3D radiusVectorOfIntersec{r0 + line.direction * t};

    return Point3D{radiusVectorOfIntersec.coords[X], radiusVectorOfIntersec.coords[Y], radiusVectorOfIntersec.coords[Z]};
}

Point3D planeLineIntersect(const Line3D& line, const Plane3D& plane)
{
    return planeLineIntersect(plane, line);
}


bool planePointIntersect(const Plane3D& plane, const Point3D& point)
{
    if (!plane.valid() || !point.valid())
        return false;

    Vector3D r0{point.coords[X], point.coords[Y], point.coords[Z]};

    if (floatZero(r0.scalarProduct(plane.norm)))
        return true;

    return false;

}

bool planePointIntersect(const Point3D& point, const Plane3D& plane)
{
    return planePointIntersect(plane, point);
}

}

