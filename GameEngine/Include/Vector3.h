#pragma once

#include "Matrix.h"

struct Vector3
{
    float   x, y, z;

    Vector3();
    Vector3(float _x, float _y, float _z);
    Vector3(const Vector3& v);
    Vector3(const XMVECTOR& v);

    // operator =
    Vector3& operator = (const Vector3& v);
    Vector3& operator = (const XMVECTOR& v);
    Vector3& operator = (float f);

    // operator ==, !=
    bool operator == (const Vector3& v) const;
    bool operator == (const XMVECTOR& v) const;
    bool operator != (const Vector3& v) const;
    bool operator != (const XMVECTOR& v) const;

    // operator []
    float operator [] (int Index)   const;
    float& operator [] (int Index);

    // operator +
    Vector3 operator + (const Vector3& v)   const;
    Vector3 operator + (const XMVECTOR& v)   const;
    Vector3 operator + (float f)    const;

    // operator +=
    Vector3 operator += (const Vector3& v);
    Vector3 operator += (const XMVECTOR& v);
    Vector3 operator += (float f);

    // operator -
    Vector3 operator - (const Vector3& v)   const;
    Vector3 operator - (const XMVECTOR& v)   const;
    Vector3 operator - (float f)    const;
    Vector3 operator - (int)    const;

    // operator -=
    Vector3 operator -= (const Vector3& v);
    Vector3 operator -= (const XMVECTOR& v);
    Vector3 operator -= (float f);

    // operator *
    Vector3 operator * (const Vector3& v)   const;
    Vector3 operator * (const XMVECTOR& v)   const;
    Vector3 operator * (float f)    const;

    // operator *=
    Vector3 operator *= (const Vector3& v);
    Vector3 operator *= (const XMVECTOR& v);
    Vector3 operator *= (float f);

    // operator /
    Vector3 operator / (const Vector3& v)   const;
    Vector3 operator / (const XMVECTOR& v)   const;
    Vector3 operator / (float f)    const;

    // operator /=
    Vector3 operator /= (const Vector3& v);
    Vector3 operator /= (const XMVECTOR& v);
    Vector3 operator /= (float f);

    float Length()  const;
    float Distance(const Vector3& v)    const;
    void Normalize();
    float Dot(const Vector3& v) const;
    float Angle(const Vector3& v)   const;
    Vector3 Cross(const Vector3& v) const;

    // w = 0
    Vector3 TransformNormal(const Matrix& m)    const;

    // w = 1
    Vector3 TransformCoord(const Matrix& m) const;


    XMVECTOR Convert()  const;
    void Convert(const XMVECTOR& v);

    static float Length(const Vector3& v);
    static float Distance(const Vector3& v, const Vector3& v1);
    static Vector3 Normalize(const Vector3& v);

    static Vector3  Zero;
    static Vector3  One;
    static Vector3  Axis[AXIS_END];
};

