#include "Vector3.h"

Vector3 Vector3::Zero;
Vector3 Vector3::One(1.f, 1.f, 1.f);
Vector3 Vector3::Axis[AXIS_END] =
{
    Vector3(1.f, 0.f, 0.f),
    Vector3(0.f, 1.f, 0.f),
    Vector3(0.f, 0.f, 1.f)
};

Vector3::Vector3() :
    x(0.f),
    y(0.f),
    z(0.f)
{
}

Vector3::Vector3(float _x, float _y, float _z) :
    x(_x),
    y(_y),
    z(_z)
{
}

Vector3::Vector3(const Vector3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

Vector3::Vector3(const XMVECTOR& v)
{
    XMStoreFloat3((XMFLOAT3*)this, v);
}

Vector3& Vector3::operator=(const Vector3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;

    return *this;
}

Vector3& Vector3::operator=(const XMVECTOR& v)
{
    XMStoreFloat3((XMFLOAT3*)this, v);

    return *this;
}

Vector3& Vector3::operator=(float f)
{
    x = f;
    y = f;
    z = f;

    return *this;
}

bool Vector3::operator==(const Vector3& v) const
{
    return (x == v.x && y == v.y && z == v.z);
}

bool Vector3::operator==(const XMVECTOR& v) const
{
    Vector3 v1(v);

    return (x == v1.x && y == v1.y && z == v1.z);
}

bool Vector3::operator!=(const Vector3& v) const
{
    return (x != v.x || y != v.y || z != v.z);
}

bool Vector3::operator!=(const XMVECTOR& v) const
{
    Vector3 v1(v);

    return (x != v1.x || y != v1.y || z != v1.z);
}

float Vector3::operator[](int Index) const
{
    switch (Index)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    }

    assert(false);
    return -1.f;
}

float& Vector3::operator[](int Index)
{
    switch (Index)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    }

    assert(false);
    return x;
}


// + ============================================
Vector3 Vector3::operator+(const Vector3& v) const
{
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator+(const XMVECTOR& v) const
{
    XMVECTOR    v1 = Convert();

    return Vector3(v + v1);
}

Vector3 Vector3::operator+(float f) const
{
    return Vector3(x + f, y + f, z + f);
}

Vector3 Vector3::operator+=(const Vector3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}

Vector3 Vector3::operator+=(const XMVECTOR& v)
{
    XMVECTOR    v1 = Convert();

    Convert(v + v1);

    return *this;
}

Vector3 Vector3::operator+=(float f)
{
    x += f;
    y += f;
    z += f;

    return *this;
}

// - ===================================================

Vector3 Vector3::operator-(const Vector3& v) const
{
    return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator-(const XMVECTOR& v) const
{
    XMVECTOR    v1 = Convert();

    return Vector3(v1 - v);
}

Vector3 Vector3::operator-(float f) const
{
    return Vector3(x - f, y - f, z - f);
}

Vector3 Vector3::operator-(int) const
{
    return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator-=(const Vector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;

    return *this;
}

Vector3 Vector3::operator-=(const XMVECTOR& v)
{
    XMVECTOR    v1 = Convert();

    Convert(v1 - v);

    return *this;
}

Vector3 Vector3::operator-=(float f)
{
    x -= f;
    y -= f;
    z -= f;

    return *this;
}


// * ===================================================

Vector3 Vector3::operator*(const Vector3& v) const
{
    return Vector3(x * v.x, y * v.y, z * v.z);
}

Vector3 Vector3::operator*(const XMVECTOR& v) const
{
    XMVECTOR    v1 = Convert();

    return Vector3(v * v1);
}

Vector3 Vector3::operator*(float f) const
{
    return Vector3(x * f, y * f, z * f);
}

Vector3 Vector3::operator*=(const Vector3& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;

    return *this;
}

Vector3 Vector3::operator*=(const XMVECTOR& v)
{
    XMVECTOR    v1 = Convert();

    Convert(v * v1);

    return *this;
}

Vector3 Vector3::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;

    return *this;
}

// / ===================================================

Vector3 Vector3::operator/(const Vector3& v) const
{
    return Vector3(x / v.x, y / v.y, z / v.z);
}

Vector3 Vector3::operator/(const XMVECTOR& v) const
{
    XMVECTOR    v1 = Convert();

    return Vector3(v1 / v);
}

Vector3 Vector3::operator/(float f) const
{
    return Vector3(x / f, y / f, z / f);
}

Vector3 Vector3::operator/=(const Vector3& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;

    return *this;
}

Vector3 Vector3::operator/=(const XMVECTOR& v)
{
    XMVECTOR    v1 = Convert();

    Convert(v1 / v);

    return *this;
}

Vector3 Vector3::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;

    return *this;
}

float Vector3::Length() const
{
    return XMVectorGetX(XMVector3Length(Convert()));
}

float Vector3::Distance(const Vector3& v) const
{
    Vector3 result = v - *this;
    return result.Length();
}

void Vector3::Normalize()
{
    Convert(XMVector3Normalize(Convert()));
}

float Vector3::Dot(const Vector3& v) const
{
    return XMVectorGetX(XMVector3Dot(Convert(), v.Convert()));
}

float Vector3::Angle(const Vector3& v) const
{
    Vector3 v1 = *this;
    Vector3 v2 = v;

    v1.Normalize();
    v2.Normalize();

    float   Angle = v1.Dot(v2);

    Angle = RadianToDegree(acosf(Angle));

    return Angle;
}

Vector3 Vector3::Cross(const Vector3& v) const
{
    return Vector3(XMVector3Cross(Convert(), v.Convert()));
}

Vector3 Vector3::TransformNormal(const Matrix& m) const
{
    return Vector3(XMVector3TransformNormal(Convert(), m.m));
}

Vector3 Vector3::TransformCoord(const Matrix& m) const
{
    return Vector3(XMVector3TransformCoord(Convert(), m.m));
}

XMVECTOR Vector3::Convert() const
{
    return XMLoadFloat3((XMFLOAT3*)this);
}

void Vector3::Convert(const XMVECTOR& v)
{
    XMStoreFloat3((XMFLOAT3*)this, v);
}

float Vector3::Length(const Vector3& v)
{
    return XMVectorGetX(XMVector3Length(v.Convert()));
}

float Vector3::Distance(const Vector3& v, const Vector3& v1)
{
    Vector3 result = v - v1;
    return result.Length();
}

Vector3 Vector3::Normalize(const Vector3& v)
{
    Vector3 v1 = v;
    v1.Normalize();
    return v1;
}

