#include "Matrix.h"
#include "Vector3.h"

Matrix::Matrix()
{
    m = XMMatrixIdentity();
}

Matrix::Matrix(const Matrix& _m)
{
    m = _m.m;
}

Matrix::Matrix(const XMMATRIX& _m)
{
    m = _m;
}

Matrix::Matrix(Vector4 _v[4])
{
    for (int i = 0; i < 4; ++i)
    {
        v[i] = _v[i];
    }
}

Vector4& Matrix::operator[](int Index)
{
    return v[Index];
}

Matrix Matrix::operator=(const Matrix& _m)
{
    m = _m.m;
    return *this;
}

Matrix Matrix::operator=(const XMMATRIX& _m)
{
    m = _m;
    return *this;
}

Matrix Matrix::operator=(Vector4 _v[4])
{
    for (int i = 0; i < 4; ++i)
    {
        v[i] = _v[i];
    }
    return *this;
}

Matrix Matrix::operator*(const Matrix& _m) const
{
    return Matrix(m * _m.m);
}

Matrix Matrix::operator*(const XMMATRIX& _m) const
{
    return Matrix(m * _m);
}

Matrix Matrix::operator*(Vector4 _v[4]) const
{
    Matrix  _m(_v);
    return Matrix(m * _m.m);
}

Matrix Matrix::operator*=(const Matrix& _m)
{
    m *= _m.m;
    return*this;
}

Matrix Matrix::operator*=(const XMMATRIX& _m)
{
    m *= _m;
    return *this;
}

Matrix Matrix::operator*=(Vector4 _v[4])
{
    Matrix  _m(_v);
    m *= _m.m;
    return *this;
}

void Matrix::Identity()
{
    m = XMMatrixIdentity();
}

void Matrix::Transpose()
{
    m = XMMatrixTranspose(m);
}

void Matrix::Inverse()
{
    XMVECTOR    Det = XMMatrixDeterminant(m);
    m = XMMatrixInverse(&Det, m);
}

void Matrix::Scaling(const Vector3& _v)
{
    m = XMMatrixScaling(_v.x, _v.y, _v.z);
}

void Matrix::Scaling(float x, float y, float z)
{
    m = XMMatrixScaling(x, y, z);
}

void Matrix::Rotation(const Vector3& _v)
{
    XMMATRIX    mX = XMMatrixRotationX(DegreeToRadian(_v.x));
    XMMATRIX    mY = XMMatrixRotationY(DegreeToRadian(_v.y));
    XMMATRIX    mZ = XMMatrixRotationZ(DegreeToRadian(_v.z));
    m = mX * mY * mZ;
}

void Matrix::Rotation(float x, float y, float z)
{
    XMMATRIX    mX = XMMatrixRotationX(DegreeToRadian(x));
    XMMATRIX    mY = XMMatrixRotationY(DegreeToRadian(y));
    XMMATRIX    mZ = XMMatrixRotationZ(DegreeToRadian(z));
    m = mX * mY * mZ;
}

void Matrix::RotationX(float x)
{
    m = XMMatrixRotationX(DegreeToRadian(x));
}

void Matrix::RotationY(float y)
{
    m = XMMatrixRotationY(DegreeToRadian(y));
}

void Matrix::RotationZ(float z)
{
    m = XMMatrixRotationZ(DegreeToRadian(z));
}

void Matrix::RotationAxis(const Vector3& Axis, float Angle)
{
    m = XMMatrixRotationAxis(Axis.Convert(), DegreeToRadian(Angle));
}

void Matrix::Translation(const Vector3& _v)
{
    m = XMMatrixTranslation(_v.x, _v.y, _v.z);
}

void Matrix::Translation(float x, float y, float z)
{
    m = XMMatrixTranslation(x, y, z);
}

Matrix Matrix::StaticIdentity()
{
    return Matrix(XMMatrixIdentity());
}

Matrix Matrix::StaticTranspose(const Matrix& _m)
{
    return Matrix(XMMatrixTranspose(_m.m));
}

Matrix Matrix::StaticInverse(const Matrix& _m)
{
    XMVECTOR    det = XMMatrixDeterminant(_m.m);
    return Matrix(XMMatrixInverse(&det, _m.m));
}

Matrix Matrix::StaticScaling(const Vector3& _v)
{
    return Matrix(XMMatrixScaling(_v.x, _v.y, _v.z));
}

Matrix Matrix::StaticScaling(float x, float y, float z)
{
    return Matrix(XMMatrixScaling(x, y, z));
}

Matrix Matrix::StaticRotation(const Vector3& _v)
{
    XMMATRIX    mX = XMMatrixRotationX(DegreeToRadian(_v.x));
    XMMATRIX    mY = XMMatrixRotationY(DegreeToRadian(_v.y));
    XMMATRIX    mZ = XMMatrixRotationZ(DegreeToRadian(_v.z));

    return Matrix(mX * mY * mZ);
}

Matrix Matrix::StaticRotation(float x, float y, float z)
{
    XMMATRIX    mX = XMMatrixRotationX(DegreeToRadian(x));
    XMMATRIX    mY = XMMatrixRotationY(DegreeToRadian(y));
    XMMATRIX    mZ = XMMatrixRotationZ(DegreeToRadian(z));

    return Matrix(mX * mY * mZ);
}

Matrix Matrix::StaticRotationX(float x)
{
    return Matrix(XMMatrixRotationX(DegreeToRadian(x)));
}

Matrix Matrix::StaticRotationY(float y)
{
    return Matrix(XMMatrixRotationY(DegreeToRadian(y)));
}

Matrix Matrix::StaticRotationZ(float z)
{
    return Matrix(XMMatrixRotationZ(DegreeToRadian(z)));
}

Matrix Matrix::StaticRotationAxis(const Vector3& Axis, float Angle)
{
    return Matrix(XMMatrixRotationAxis(Axis.Convert(), Angle));
}

Matrix Matrix::StaticTranslation(const Vector3& _v)
{
    return Matrix(XMMatrixTranslation(_v.x, _v.y, _v.z));
}

Matrix Matrix::StaticTranslation(float x, float y, float z)
{
    return Matrix(XMMatrixTranslation(x, y, z));
}
