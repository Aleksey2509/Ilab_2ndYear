#ifndef GEOMETRY_3D_HH
#define GEOMETRY_3D_HH


#include <cmath>
#include <limits>


namespace geometry3D
{

    enum Axis
    {
        X,
        Y,
        Z
    };

    using float_t = double;

    constexpr float_t EPS = 1e-6;
    constexpr auto nan = std::numeric_limits<float_t>::quiet_NaN();
    constexpr auto inf = std::numeric_limits<float_t>::infinity();

    bool floatValid(float_t value)
    {
        return(!std::isnan(value) && !std::isinf(value));
    }

    bool floatEqual(float_t lhs, float_t rhs)
    {
        return std::abs(lhs - rhs) < EPS;
    }

    bool floatZero(float_t lhs)
    {
        return floatEqual(lhs, 0.0);
    }

struct Point3D
{
    float_t coords[3] = {nan, nan, nan};

    bool valid() const
    {
        return floatValid(coords[X]) && floatValid(coords[Y]) && floatValid(coords[Z]);
    }

    void setInvalid()
    {
        coords[X] = coords[Y] = coords[Z] = nan;
    }

    bool operator == (const Point3D& other) const
    {
        if ((valid() && !other.valid()) || (!valid() && other.valid()))
            return false;

        if (!valid() && !other.valid())
            return true;

        return (floatEqual(coords[X], other.coords[X])) &&
               (floatEqual(coords[Y], other.coords[Y])) &&
               (floatEqual(coords[Z], other.coords[Z]));
    }

    Point3D& operator =(const Point3D& rhs)
    {
        coords[X] = rhs.coords[X];
        coords[Y] = rhs.coords[Y];
        coords[Z] = rhs.coords[Z];

        return *this;
    }
};

struct Vector3D
{
    float_t coords[3] = {nan, nan, nan};

    bool valid() const
    {
        return floatValid(coords[X]) && floatValid(coords[Y]) && floatValid(coords[Z]);
    }

    Vector3D(float_t firstCoord, float_t secondCoord, float_t thirdCoord)
    {
        coords[X] = firstCoord;
        coords[Y] = secondCoord;
        coords[Z] = thirdCoord;
    }

    Vector3D(Point3D start, Point3D end) : Vector3D{end.coords[X] - start.coords[X],
                                                    end.coords[Y] - start.coords[Y],
                                                    end.coords[Z] - start.coords[Z]}
    {}

    Vector3D& makeUnit()
    {
        if (valid())
        {
            float_t module = std::sqrt(scalarProduct(*this));
            (*this) /= module;
        }
        return *this;
    }

    Vector3D& operator += (const Vector3D& other)
    {
        coords[X] += other.coords[X];
        coords[Y] += other.coords[Y];
        coords[Z] += other.coords[Z];

        return *this;
    }

    Vector3D& operator -= (const Vector3D& other)
    {
        coords[X] -= other.coords[X];
        coords[Y] -= other.coords[Y];
        coords[Z] -= other.coords[Z];

        return *this;
    }

    Vector3D& operator *= (float_t scale)
    {
        coords[X] *= scale;
        coords[Y] *= scale;
        coords[Z] *= scale;

        return *this;
    }

    Vector3D& operator /= (float_t scale)
    {
        coords[X] /= scale;
        coords[Y] /= scale;
        coords[Z] /= scale;

        return *this;
    }

    float_t scalarProduct(const Vector3D& other) const
    {
        return coords[X] * other.coords[X] + coords[Y] * other.coords[Y] + coords[Z] * other.coords[Z]; 
    }

    Vector3D crossProduct(const Vector3D& other) const
    {
        return Vector3D{coords[Y] * other.coords[Z] - coords[Z] * other.coords[Y],
                        coords[Z] * other.coords[X] - coords[X] * other.coords[Z],
                        coords[X] * other.coords[Y] - coords[Y] * other.coords[X]};
    }

    bool operator == (const Vector3D& other) const
    {
            return (floatEqual(coords[X], other.coords[X])) &&
                   (floatEqual(coords[Y], other.coords[Y])) &&
                   (floatEqual(coords[Z], other.coords[Z]));
    }

};

Vector3D operator+ (const Vector3D& lhs, const Vector3D& rhs)
{
    Vector3D tmp{lhs};
    return tmp += rhs;
}

Vector3D operator- (const Vector3D& lhs, const Vector3D& rhs)
{
    Vector3D tmp{lhs};
    return tmp -= rhs;
}

Vector3D operator * (float_t scale, const Vector3D& rhs)
{
    Vector3D tmp{rhs};
    return tmp *= scale;
}

Vector3D operator * (const Vector3D& rhs, float_t scale)
{
    Vector3D tmp{rhs};
    return tmp *= scale;
}

struct Segment3D
{
    Point3D a;
    Point3D b;

    Segment3D(Point3D first, Point3D second) : a(first), b(second)
    {}

    float_t len() const
    {
        return std::sqrt((a.coords[X] - b.coords[X]) * (a.coords[X] - b.coords[X]) +
                         (a.coords[Y] - b.coords[Y]) * (a.coords[Y] - b.coords[Y]) +
                         (a.coords[Z] - b.coords[Z]) * (a.coords[Z] - b.coords[Z]));
    }


    #if MAKE_GOOD_FILE_DISTRIB
        bool containsPoint(const Point3D& toCheck) const
        {
            if(!Line3D{a, b}.containsPoint(toCheck))
                return false;

            return Vector3D{a, toCheck}.scalarProduct(Vector3D{b, toCheck}) < 0;
        }
    #endif
};

struct Line3D
{
    Vector3D direction;
    Point3D point;

    Line3D(Vector3D vec, Point3D startingPoint) : direction(vec), point(startingPoint)
    {}

    Line3D(Point3D first, Point3D second) : Line3D{Vector3D{first, second}, second}
    {}

    Line3D(Segment3D segment) : Line3D{segment.a, segment.b}
    {}

    bool valid() const
    {
        return direction.valid() && point.valid();
    }

    bool collinearToVector(const Vector3D& rhs) const
    {
        if (direction.crossProduct(rhs) == Vector3D{0.0, 0.0, 0.0})
            return true;

        return false;
    }

    bool containsPoint(const Point3D& toCheck) const
    {
        if (Line3D{point, toCheck}.collinearToVector(direction))
            return true;
        
        return false;
    }

    bool containsSegment(const Segment3D& toCheck) const
    {
        return containsPoint(toCheck.a) && containsPoint(toCheck.b);
    }

};

// (norm * r) + d = 0, where * - dot product
struct Plain3D
{
    Vector3D norm;
    float_t d;

    Plain3D(Vector3D normal, float_t freeCoef) : norm(normal.makeUnit()), d(freeCoef)
    {}

    bool operator== (const Plain3D& other) const
    {
        return (norm == other.norm) && (d == other.d);
    }

    bool valid() const
    {
        return norm.valid() && floatValid(d);
    }
};


// needs explanation comment

// okay now, have you forgottend where right and left is?
Point3D lineLineIntersect(const Line3D rhs, const Line3D lhs)
{
    if (!rhs.valid() || !lhs.valid())
        return Point3D{};

    if (rhs.collinearToVector(lhs.direction))
        return Point3D{};


    Vector3D startingPointsDifference{rhs.point, lhs.point};
    if (!floatZero(startingPointsDifference.scalarProduct(rhs.direction.crossProduct(lhs.direction))))
        return Point3D{};

    float_t det = rhs.direction.coords[Y] * lhs.direction.coords[X] - rhs.direction.coords[X] * lhs.direction.coords[Y];

    float_t detT1 = rhs.direction.coords[Y] * (lhs.point.coords[X] - rhs.point.coords[X]) -
                    rhs.direction.coords[X] * (lhs.point.coords[Y] - rhs.point.coords[Y]);

    float_t t1 = detT1 / det;
    Vector3D r0{rhs.point.coords[X], rhs.point.coords[Y], rhs.point.coords[Z]};

    Vector3D radiusVectorOfIntersec{r0 + rhs.direction * t1};

    return Point3D{radiusVectorOfIntersec.coords[X], radiusVectorOfIntersec.coords[Y], radiusVectorOfIntersec.coords[Z]};
}

// needs explanation comment
Point3D planeLineIntersect(const Plain3D& plain, const Line3D& line)
{
    if (!plain.valid() || !line.valid())
        return Point3D{};

    if (floatZero(plain.norm.scalarProduct(line.direction)))
        return Point3D{};

    Vector3D r0{line.point.coords[X], line.point.coords[Y], line.point.coords[Z]};

    float_t t = (plain.norm.scalarProduct(r0) + plain.d) / (plain.norm.scalarProduct(line.direction));

    Vector3D radiusVectorOfIntersec{r0 + line.direction * t};

    return Point3D{radiusVectorOfIntersec.coords[X], radiusVectorOfIntersec.coords[Y], radiusVectorOfIntersec.coords[Z]};
}

Point3D planeLineIntersect(const Line3D& line, const Plain3D& plain)
{
    return planeLineIntersect(plain, line);
}

bool planePointIntersect(const Plain3D& plain, const Point3D& point)
{
    if (!plain.valid() || !point.valid())
        return false;

    Vector3D r0{point.coords[X], point.coords[Y], point.coords[Z]};

    if (floatZero(r0.scalarProduct(plain.norm)))
        return true;

    return false;

}

bool planePointIntersect(const Point3D& point, const Plain3D& plain)
{
    return planePointIntersect(plain, point);
}

class Triangle3D
{
    Point3D points_[3];

    Segment3D ab;
    Segment3D bc;
    Segment3D ac;

    bool valid_ = false;

public:

    Triangle3D(Point3D a, Point3D b, Point3D c) : ab(a, b), bc(b, c), ac(a, c)
    {
        points_[0] = a;
        points_[1] = b;
        points_[2] = c;
    }

    
};


}


#endif