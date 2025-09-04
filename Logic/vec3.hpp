#ifndef VEC3_H
#define VEC3_H
#include <iostream>
#include <cmath>
// #include <bits/stdc++.h>
#include <functional>

class Vec3
{
private:
    float e[3];

public:
    Vec3() : e{0, 0, 0} {}
    Vec3(float x, float y, float z) : e{x, y, z} {}
    ~Vec3() = default;
    Vec3 &operator=(const Vec3 &v)
    {
        e[0] = v[0];
        e[1] = v[1];
        e[2] = v[2];
        return *this;
    }

    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }
    Vec3 operator-() const
    {
        return Vec3(-e[0], -e[1], -e[2]);
    }
    float operator[](int i) const { return e[i]; }
    float &operator[](int i) { return e[i]; }

    Vec3 &operator+=(const Vec3 &v)
    {
        e[0] += v[0];
        e[1] += v[1];
        e[2] += v[2];
        return *this;
    }
    Vec3 &operator-=(const Vec3 &v)
    {
        e[0] -= v[0];
        e[1] -= v[1];
        e[2] -= v[2];
        return *this;
    }

    Vec3 &operator*=(const float d)
    {
        e[0] *= d;
        e[1] *= d;
        e[2] *= d;
        return *this;
    }

    Vec3 &operator/=(const float d)
    {
        e[0] *= 1.0f / d;
        e[1] *= 1.0f / d;
        e[2] *= 1.0f / d;
        return *this;
    }

    bool operator==(const Vec3 &v) const
    {
        return ((e[0] == v[0]) && (e[1] == v[1]) && (e[2] == v[2]));
    }

    bool operator<(const Vec3 &v) const

    {
        // return e[0] < v.x() || (e[0] == v.x() && e[1] < v.y()) || (e[0] == v.x() && e[1] == v.y() && e[2] < v.z());
        if (e[0] != v.x())
        {
            return e[0] < v.x();
        }
        if (e[1] != v.y())
        {
            return e[1] < v.y();
        }
        return e[2] < v.z();
    }

    // bool operator>(Vec3 const &v)
    // {
    //     return e[0] > v.x() || (e[0] == v.x() && e[1] > v.y()) || (e[0] == v.x() && e[1] == v.y() && e[2] > v.z());
    // }

    float len() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
};

inline std::ostream &operator<<(std::ostream &out, const Vec3 &v)
{
    return out << v[0] << ' ' << v[1] << ' ' << v[2] << std::endl;
}

inline Vec3 operator+(const Vec3 &v0, const Vec3 &v1)
{
    return Vec3(v0[0] + v1[0], v0[1] + v1[1], v0[2] + v1[2]);
}

inline Vec3 operator-(const Vec3 &v0, const Vec3 &v1)
{
    return Vec3(v0[0] - v1[0], v0[1] - v1[1], v0[2] - v1[2]);
}

inline Vec3 operator*(const Vec3 &v0, const Vec3 &v1)
{
    return Vec3(v0[0] * v1[0], v0[1] * v1[1], v0[2] * v1[2]);
}

inline Vec3 operator*(const Vec3 &v, float t)
{
    return Vec3(v[0] * t, v[1] * t, v[2] * t);
}

inline Vec3 operator*(float t, const Vec3 &v)
{
    return Vec3(v[0] * t, v[1] * t, v[2] * t);
}

inline Vec3 operator/(const Vec3 &v, float t)
{
    return Vec3(v[0] / t, v[1] / t, v[2] / t);
}

inline float dot(const Vec3 &v0, const Vec3 &v1)
{
    return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
}

inline Vec3 cross(const Vec3 &v0, const Vec3 &v1)
{
    return Vec3(v0[1] * v1[2] - v0[2] * v1[1],
                v0[2] * v1[0] - v0[0] * v1[2],
                v0[0] * v1[1] - v0[1] * v1[0]);
}

inline Vec3 unit(const Vec3 &v)
{
    return v / v.len();
}

inline Vec3 maxCompare(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(std::max(v1[0], v2[0]), std::max(v1[1], v2[1]), std::max(v1[2], v2[2]));
}

inline Vec3 minCompare(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(std::min(v1[0], v2[0]), std::min(v1[1], v2[1]), std::min(v1[2], v2[2]));
}

inline Vec3 normalize(const Vec3 &v)
{
    float bigger = -INFINITY;
    if (v.x() > bigger)
    {
        bigger = v.x();
    }
    if (v.y() > bigger)
    {
        bigger = v.y();
    }
    if (v.z() > bigger)
    {
        bigger = v.z();
    }

    return Vec3(v.x() / bigger, v.y() / bigger, v.z() / bigger);
}

class MyHashFunction
{
public:
    // We use predefined hash functions of strings
    // and define our hash function as XOR of the
    // hash values.
    size_t operator()(const Vec3 &p) const
    {
        return ((std::hash<float>()(p.x())) ^ (std::hash<float>()(p.y())) ^ (std::hash<float>()(p.z())));
    }
};
using Point3 = Vec3;
using RGBcolor = Vec3;

#endif
