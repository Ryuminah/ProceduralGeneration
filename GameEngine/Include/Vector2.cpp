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

XMVECTOR Vector2::Convert() const
{
    return XMLoadFloat2((XMFLOAT2*)this);
}

void Vector2::Convert(const XMVECTOR& v)
{
    XMStoreFloat2((XMFLOAT2*)this, v);
}
