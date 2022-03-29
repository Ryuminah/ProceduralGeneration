#include "Vector2.h"

Vector2::Vector2()  :
    x(0.f),
    y(0.f)
{
}

Vector2::Vector2(float _x, float _y)    :
    x(_x),
    y(_y)
{
}

Vector2::Vector2(const Vector2& v)
{
    x = v.x;
    y = v.y;
}

Vector2::Vector2(const XMVECTOR& v)
{
    XMStoreFloat2((XMFLOAT2*)this, v);
}

Vector2& Vector2::operator=(const Vector2& v)
{
    x = v.x;
    y = v.y;

    return *this;
}

Vector2& Vector2::operator=(const XMVECTOR& v)
{
    XMStoreFloat2((XMFLOAT2*)this, v);

    return *this;
}

Vector2& Vector2::operator=(float f)
{
    x = f;
    y = f;

    return *this;
}

bool Vector2::operator==(const Vector2& v) const
{
    return (x == v.x && y == v.y);
}

bool Vector2::operator==(const XMVECTOR& v) const
{
    Vector2 v1(v);

    return (x == v1.x && y == v1.y);
}

bool Vector2::operator!=(const Vector2& v) const
{
    return (x != v.x || y != v.y);
}

bool Vector2::operator!=(const XMVECTOR& v) const
{
    Vector2 v1(v);

    return (x != v1.x || y != v1.y);
}

float Vector2::operator[](int Index) const
{
    switch (Index)
    {
    case 0:
        return x;
    case 1:
        return y;
    }

    assert(false);
    return -1.f;
}

float& Vector2::operator[](int Index)
{
    switch (Index)
    {
    case 0:
        return x;
    case 1:
        return y;
    }

    assert(false);
    return x;
}


// + ============================================
Vector2 Vector2::operator+(const Vector2& v) const
{
    return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator+(const XMVECTOR& v) const
{
    XMVECTOR    v1 = Convert();

    return Vector2(v + v1);
}

Vector2 Vector2::operator+(float f) const
{
    return Vector2(x + f, y + f);
}

Vector2 Vector2::operator+=(const Vector2& v)
{
    x += v.x;
    y += v.y;

    return *this;
}

Vector2 Vector2::operator+=(const XMVECTOR& v)
{
    XMVECTOR    v1 = Convert();

    Convert(v + v1);

    return *this;
}

Vector2 Vector2::operator+=(float f)
{
    x += f;
    y += f;

    return *this;
}

// - ===================================================

Vector2 Vector2::operator-(const Vector2& v) const
{
    return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator-(const XMVECTOR& v) const
{
    XMVECTOR    v1 = Convert();

    return Vector2(v1 - v);
}

Vector2 Vector2::operator-(float f) const
{
    return Vector2(x - f, y - f);
}

Vector2 Vector2::operator-(int) const
{
    return Vector2(-x, -y);
}

Vector2 Vector2::operator-=(const Vector2& v)
{
    x -= v.x;
    y -= v.y;

    return *this;
}

Vector2 Vector2::operator-=(const XMVECTOR& v)
{
    XMVECTOR    v1 = Convert();

    Convert(v1 - v);

    return *this;
}

Vector2 Vector2::operator-=(float f)
{
    x -= f;
    y -= f;

    return *this;
}


// * ===================================================

Vector2 Vector2::operator*(const Vector2& v) const
{
    return Vector2(x * v.x, y * v.y);
}

Vector2 Vector2::operator*(const XMVECTOR& v) const
{
    XMVECTOR    v1 = Convert();

    return Vector2(v * v1);
}

Vector2 Vector2::operator*(float f) const
{
    return Vector2(x * f, y * f);
}

Vector2 Vector2::operator*=(const Vector2& v)
{
    x *= v.x;
    y *= v.y;

    return *this;
}

Vector2 Vector2::operator*=(const XMVECTOR& v)
{
    XMVECTOR    v1 = Convert();

    Convert(v * v1);

    return *this;
}

Vector2 Vector2::operator*=(float f)
{
    x *= f;
    y *= f;

    return *this;
}

// / ===================================================

Vector2 Vector2::operator/(const Vector2& v) const
{
    return Vector2(x / v.x, y / v.y);
}

Vector2 Vector2::operator/(const XMVECTOR& v) const
{
    XMVECTOR    v1 = Convert();

    return Vector2(v1 / v);
}

Vector2 Vector2::operator/(float f) const
{
    return Vector2(x / f, y / f);
}

Vector2 Vector2::operator/=(const Vector2& v)
{
    x /= v.x;
    y /= v.y;

    return *this;
}

Vector2 Vector2::operator/=(const XMVECTOR& v)
{
    XMVECTOR    v1 = Convert();

    Convert(v1 / v);

    return *this;
}

Vector2 Vector2::operator/=(float f)
{
    x /= f;
    y /= f;

    return *this;
}

float Vector2::Length() const
{
    return XMVectorGetX(XMVector2Length(Convert()));
}

float Vector2::Distance(const Vector2& v) const
{
    Vector2 result = v - *this;
    return result.Length();
}

void Vector2::Normalize()
{
    float VectorLength = Length();
    x /= VectorLength;
    y /= VectorLength;
}

float Vector2::Dot(const Vector2& v) const
{
    return x * v.x + y * v.y;
}

float Vector2::Cross(const Vector2& v) const
{
    float result = (x * v.y) - (y * v.x);
    return result;
}

int Vector2::CheckIntersect(const Vector2 pointSrc1, const Vector2 pointSrc2, const Vector2 CheckPoint)
{
    float result = (pointSrc2.y - pointSrc1.y) * (CheckPoint.x - pointSrc2.x) - (pointSrc2.x - pointSrc1.x);

    return result < 0 ? -1 : (result ? 1 : 0);
}

bool Vector2::IsPointinLine(const Vector2 pointSrc1, const Vector2 pointSrc2, const Vector2 CheckPoint)
{
    if (pointSrc1.x != pointSrc2.x && pointSrc1.y != pointSrc2.y)
    {
        float fResult = (pointSrc2.y - pointSrc2.y) / ((float)pointSrc2.x - pointSrc1.x), y;
        y = fResult * (CheckPoint.x - pointSrc1.x) + pointSrc1.y;

        return pointSrc1.x <= CheckPoint.x && CheckPoint.x <= pointSrc2.x && y == CheckPoint.y;
    }

    if (pointSrc1.x == pointSrc2.x)
    {
        return pointSrc1.x == CheckPoint.x && pointSrc1.y <= CheckPoint.y && CheckPoint.y == pointSrc2.y;
    }

    else
    {
        return CheckPoint.y == pointSrc1.y && pointSrc1.x <= CheckPoint.x && CheckPoint.x <= pointSrc2.x;
    }
}

bool Vector2::IsIntersectLines(const Vector2 pointSrc1, const Vector2 pointSrc2, const Vector2 PointDest1, const Vector2 PointDest2)
{
    bool result = false;

    int Line1, Line2;

    // 비교할 선분과 점이 교차하는 지 확인
    Line1 = CheckIntersect(pointSrc1, pointSrc2, PointDest1);
    Line2 = CheckIntersect(pointSrc1, pointSrc2, PointDest2);

    if ((Line1 != Line2 && CheckIntersect(PointDest1, PointDest2, pointSrc1) != CheckIntersect(PointDest1, PointDest2, pointSrc2)))
    {
        result = true;
    }

    // 평행일 경우 기준 선분에 다른 선분의 점이 들어있는지 판별한다.
    else if (!Line1 && !Line2)
    {
        result = IsPointinLine(pointSrc1, pointSrc2, PointDest1) || IsPointinLine(pointSrc1, pointSrc2, PointDest2)
            || IsPointinLine(PointDest1, PointDest2, pointSrc1) || IsPointinLine(PointDest1, PointDest2, pointSrc2);
    }

    return result;
}

bool Vector2::GetIntersectPoint(const Vector2 pointSrc1, const Vector2 pointSrc2, const Vector2 PointDest1, const Vector2 PointDest2, Vector2& intersectPoint)
{
    float t;
    float s;
    float fCheckIntersect = (PointDest2.y - PointDest1.y) * (pointSrc2.x - pointSrc1.x) - (PointDest2.x - PointDest1.x) * (pointSrc2.y - pointSrc1.y);

    // 교차하지 않는 상황
    if (fCheckIntersect == 0)
    {
        return false;
    }

    float _t = (float)(PointDest2.x - PointDest1.x) * (pointSrc1.y - PointDest1.y) - (PointDest2.y - PointDest1.y) * (pointSrc1.x - PointDest1.x);
    float _s = (float)(pointSrc2.x - pointSrc1.x) * (pointSrc1.y - PointDest1.y) - (pointSrc2.y - pointSrc1.y) * (pointSrc1.x - PointDest1.x);

    t = _t / fCheckIntersect;
    s = _s / fCheckIntersect;


    if (t < 0.0 || t>1.0 || s < 0.0 || s>1.0)
    {
        return false;
    }

    if (_t == 0 && _s == 0)
    {
        return false;
    }

    // 교차하는 경우
    intersectPoint.x = pointSrc1.x + t * (float)(pointSrc2.x - pointSrc1.x);
    intersectPoint.y = pointSrc1.y + t * (float)(pointSrc2.y - pointSrc1.y);

    return true;
}

float Vector2::DistanceOfPointAndLine(Vector2 LineStartPoint, Vector2 LineEndPoint, Vector2 Point)
{
    Vector2 Line = LineEndPoint - LineStartPoint;
    Vector2 v1 = Point - LineStartPoint;
    Vector2 v2 = Point - LineEndPoint;

    float Distance = v1.Dot(v2)/ Line.Length();

    return Distance;
}

XMVECTOR Vector2::Convert() const
{
    return XMLoadFloat2((XMFLOAT2*)this);
}

void Vector2::Convert(const XMVECTOR& v)
{
    XMStoreFloat2((XMFLOAT2*)this, v);
}
