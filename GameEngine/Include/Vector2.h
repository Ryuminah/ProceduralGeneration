#pragma once

#include "EngineMath.h"

struct Vector2
{
    float   x, y;

    Vector2();
    Vector2(float _x, float _y);
    Vector2(const Vector2& v);
    Vector2(const XMVECTOR& v);

    // operator =
    Vector2& operator = (const Vector2& v);
    Vector2& operator = (const XMVECTOR& v);
    Vector2& operator = (float f);

    // operator ==, !=
    bool operator == (const Vector2& v) const;
    bool operator == (const XMVECTOR& v) const;
    bool operator != (const Vector2& v) const;
    bool operator != (const XMVECTOR& v) const;

    // operator []
    float operator [] (int Index)   const;
    float& operator [] (int Index);

    // operator +
    Vector2 operator + (const Vector2& v)   const;
    Vector2 operator + (const XMVECTOR& v)   const;
    Vector2 operator + (float f)    const;

    // operator +=
    Vector2 operator += (const Vector2& v);
    Vector2 operator += (const XMVECTOR& v);
    Vector2 operator += (float f);

    // operator -
    Vector2 operator - (const Vector2& v)   const;
    Vector2 operator - (const XMVECTOR& v)   const;
    Vector2 operator - (float f)    const;
    Vector2 operator - (int)    const;

    // operator -=
    Vector2 operator -= (const Vector2& v);
    Vector2 operator -= (const XMVECTOR& v);
    Vector2 operator -= (float f);

    // operator *
    Vector2 operator * (const Vector2& v)   const;
    Vector2 operator * (const XMVECTOR& v)   const;
    Vector2 operator * (float f)    const;

    // operator *=
    Vector2 operator *= (const Vector2& v);
    Vector2 operator *= (const XMVECTOR& v);
    Vector2 operator *= (float f);

    // operator /
    Vector2 operator / (const Vector2& v)   const;
    Vector2 operator / (const XMVECTOR& v)   const;
    Vector2 operator / (float f)    const;

    // operator /=
    Vector2 operator /= (const Vector2& v);
    Vector2 operator /= (const XMVECTOR& v);
    Vector2 operator /= (float f);

    float Length()  const;
    float Distance(const Vector2& v)    const;
    void Normalize();
    float Dot(const Vector2& v) const;
    XMVECTOR Convert()  const;
    void Convert(const XMVECTOR& v);
};

