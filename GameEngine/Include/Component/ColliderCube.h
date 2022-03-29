#pragma once
#include "Collider.h"

// 선분을 이루는 양끝 두 점 
struct LinePoint
{
    Vector3 Point[2];

    std::vector<Vector2> GetPointVector3ToVector2()
    {
        std::vector<Vector2> result;
        Vector2 pointX = { Point[0].x, Point[0].z };
        Vector2 pointY = { Point[1].x, Point[1].z };

        result.push_back(pointX);
        result.push_back(pointY);

        return result;
    }
};

class CColliderCube :
    public CCollider
{
    friend class CGameObject;

protected:
    CColliderCube();
    CColliderCube(const CColliderCube& com);
    virtual ~CColliderCube();

protected:
    CubeInfo m_Info;
    Vector3 m_BottomPoint[4];
    LinePoint VerticalLineToFoward;

public:
    CubeInfo GetInfo()   const
    {
        return m_Info;
    }

    void GetBottomPoint(std::vector<Vector3>& bottomPoint)
    {
        for (int i = 0; i < 4; ++i)
        {
            bottomPoint.push_back(m_BottomPoint[i]);
        }
    }

    LinePoint GetVerticalLineToFoward()
    {
        return VerticalLineToFoward;
    }


public:
    void SetVolume(float Width, float Length, float Height)
    {
        m_Info.Length[0] = Width;
        m_Info.Length[1] = Length;
        m_Info.Length[2] = Height;

        SetWorldScale(m_Info.Length[0] * 2.f, m_Info.Length[1] * 2.f, m_Info.Length[2] * 2.f);
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void PrevRender(float DeltaTime);
    virtual void Render(float DeltaTime);
    virtual CColliderCube* Clone();

public:
    virtual bool Collision(CCollider* Dest);
    virtual bool CollisionMouse(const Vector2& MousePos);

public:
    // 사각형 충돌 제작에 필요한 함수들
    void VerticalToFowardVector();
    std::vector<Vector2> GetNormalVectorToLine(const Vector2& pointStart, const Vector2& pointEnd);
    void CaculateVertexPos();

    bool IsIntersectPlaneAndLine(Vector3 Line);




};
