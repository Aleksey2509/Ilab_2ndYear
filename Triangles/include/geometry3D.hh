#ifndef GEOMETRY_3D_HH
#define GEOMETRY_3D_HH


#include <cmath>
#include <limits>
#include <array>


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


    inline bool floatValid(float_t value)
    {
        return(!std::isnan(value) && !std::isinf(value));
    }

    inline bool floatEqual(float_t lhs, float_t rhs)
    {
        return std::abs(lhs - rhs) < EPS;
    }

    inline bool floatZero(float_t lhs)
    {
        return floatEqual(lhs, 0.0);
    }

struct Point3D
{
    std::array<float_t, 3> coords = {nan, nan, nan};

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
    std::array<float_t, 3> coords = {nan, nan, nan};

    bool valid() const
    {
        return floatValid(coords[X]) && floatValid(coords[Y]) && floatValid(coords[Z]);
    }

    Vector3D() = default;

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

    Vector3D operator+ (const Vector3D& rhs)
    {
        Vector3D tmp{*this};
        return tmp += rhs;
    }

    Vector3D operator- (const Vector3D& rhs)
    {
        Vector3D tmp{*this};
        return tmp -= rhs;
    }

};


inline Vector3D operator * (float_t scale, const Vector3D& rhs)
{
    Vector3D tmp{rhs};
    return tmp *= scale;
}

inline Vector3D operator * (const Vector3D& rhs, float_t scale)
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

    Line3D() = default;

    Line3D(Vector3D vec, Point3D startingPoint) : direction(vec), point(startingPoint)
    {}

    Line3D(Segment3D segment) : Line3D{Vector3D{segment.a, segment.b}, segment.b}
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
        if (Line3D{Vector3D{point, toCheck}, toCheck}.collinearToVector(direction))
            return true;
        
        return false;
    }

    bool containsSegment(const Segment3D& toCheck) const
    {
        return containsPoint(toCheck.a) && containsPoint(toCheck.b);
    }

};

// (norm * r) + d = 0, where * - dot product
struct Plane3D
{
    Vector3D norm;
    float_t d;

    Plane3D(Vector3D normal, float_t freeCoef) : norm(normal.makeUnit()), d(freeCoef)
    {}

    bool operator== (const Plane3D& other) const
    {
        return (norm == other.norm) && (d == other.d);
    }

    bool valid() const
    {
        return norm.valid() && floatValid(d);
    }
};

class Triangle3D
{
    Point3D points_[3];

    Segment3D ab;
    Segment3D bc;
    Segment3D ac;

    // bool valid_ = false;

public:

    Triangle3D(Point3D a, Point3D b, Point3D c) : ab(a, b), bc(b, c), ac(a, c)
    {
        points_[0] = a;
        points_[1] = b;
        points_[2] = c;
    }

    
};

Point3D lineLineIntersect(const Line3D lhs, const Line3D rhs);
Point3D planeLineIntersect(const Plane3D& plain, const Line3D& line);
Point3D planeLineIntersect(const Line3D& line, const Plane3D& plain);
bool planePointIntersect(const Plane3D& plain, const Point3D& point);
bool planePointIntersect(const Point3D& point, const Plane3D& plain);

}


#endif