#pragma once

#include "EngineMath.h"

struct Vector4
{
    float   x, y, z, w;

    Vector4();
    Vector4(float _x, float _y, float _z, float _w);
    Vector4(const Vector4& v);
    Vector4(const XMVECTOR& v);

    // operator =
    Vector4& operator = (const Vector4& v);
    Vector4& operator = (const XMVECTOR& v);
    Vector4& operator = (float f);

    // operator ==, !=
    bool operator == (const Vector4& v) const;
    bool operator == (const XMVECTOR& v) const;
    bool operator != (const Vector4& v) const;
    bool operator != (const XMVECTOR& v) const;

    // operator []
    float operator [] (int Index)   const;
    float& operator [] (int Index);

    // operator +
    Vector4 operator + (const Vector4& v)   const;
    Vector4 operator + (const XMVECTOR& v)   const;
    Vector4 operator + (float f)    const;

    // operator +=
    Vector4 operator += (const Vector4& v);
    Vector4 operator += (const XMVECTOR& v);
    Vector4 operator += (float f);

    // operator -
    Vector4 operator - (const Vector4& v)   const;
    Vector4 operator - (const XMVECTOR& v)   const;
    Vector4 operator - (float f)    const;
    Vector4 operator - (int)    const;

    // operator -=
    Vector4 operator -= (const Vector4& v);
    Vector4 operator -= (const XMVECTOR& v);
    Vector4 operator -= (float f);

    // operator *
    Vector4 operator * (const Vector4& v)   const;
    Vector4 operator * (const XMVECTOR& v)   const;
    Vector4 operator * (float f)    const;

    // operator *=
    Vector4 operator *= (const Vector4& v);
    Vector4 operator *= (const XMVECTOR& v);
    Vector4 operator *= (float f);

    // operator /
    Vector4 operator / (const Vector4& v)   const;
    Vector4 operator / (const XMVECTOR& v)   const;
    Vector4 operator / (float f)    const;

    // operator /=
    Vector4 operator /= (const Vector4& v);
    Vector4 operator /= (const XMVECTOR& v);
    Vector4 operator /= (float f);

    XMVECTOR Convert()  const;
    void Convert(const XMVECTOR& v);

    static Vector4  Black;
    static Vector4  White;
    static Vector4  Red;
    static Vector4  Green;
    static Vector4  Blue;

};

