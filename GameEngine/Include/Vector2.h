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
    float Cross(const Vector2& v) const;

    // 교차 판별
    static int CheckIntersect(const Vector2 pointSrc1, const Vector2 pointSrc2, const Vector2 CheckPoint);

    // 두 점이 이루는 선분 위에 어떤 점이 있는지 판단
    static bool IsPointinLine(const Vector2 pointSrc1, const Vector2 pointSrc2, const Vector2 CheckPoint);

    // 두 점이 이루는 선분 두개가 교차하는지 판단
    static bool IsIntersectLines(const Vector2 pointSrc1, const Vector2 pointSrc2, const Vector2 PointDest1, const Vector2 PointDest2);

    // 두 선분이 교차하는지 판단하고, 교점을 가져옴
    static bool GetIntersectPoint(const Vector2 pointSrc1, const Vector2 pointSrc2, const Vector2 PointDest1, const Vector2 PointDest2, Vector2& intersectPoint);

    // 점과 직선 사이의 최단거리 구하기
    static float DistanceOfPointAndLine(Vector2 LineStartPoint, Vector2 LineEndPoint, Vector2 Point);

    XMVECTOR Convert()  const;
    void Convert(const XMVECTOR& v);
};

