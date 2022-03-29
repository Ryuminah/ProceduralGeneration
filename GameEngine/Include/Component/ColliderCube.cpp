#include "ColliderCube.h"
#include "../Scene/Scene.h"
#include "../Collision.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"
#include "../Render/RenderStateManager.h"

CColliderCube::CColliderCube()
{
    m_ColliderShape = Collider_Shape::Cube;
    m_ColliderSpace = Collider_Space::Collider3D;

    m_PrimitiveType = PrimitiveComponent_Type::Primitive3D;
    m_3DType = RT3D_Default;

    for (int i = 0; i < 3; ++i)
    {
        m_Info.Length[i] = 0.5f;
    }
}

CColliderCube::CColliderCube(const CColliderCube& com) :
    CCollider(com)
{
    m_Info = com.m_Info;
}

CColliderCube::~CColliderCube()
{

}

void CColliderCube::Start()
{
    CCollider::Start();

    SetWorldScale(m_Info.Length[0] * 2.f, m_Info.Length[1] * 2.f, m_Info.Length[2] * 2.f);

    m_DebugMesh = m_pScene->GetResource()->FindMesh("CubeLine");

    CaculateVertexPos();
    VerticalToFowardVector();
}

bool CColliderCube::Init()
{
    if (!CCollider::Init())
        return false;

 /*   SetUpdatePosZ(true);
    SetInheritRotZ(true);*/
    SetInheritRotY(true);


    m_WireFrame = CRenderStateManager::GetInst()->FindRenderState("WireFrame");
    m_DebugShader = CShaderManager::GetInst()->FindShader("Collider3DShader");

    m_Info.Axis[AXIS_X] = Vector3(1.f, 0.f, 0.f);
    m_Info.Axis[AXIS_Y] = Vector3(0.f, 1.f, 0.f);
    m_Info.Axis[AXIS_Z] = Vector3(0.f, 0.f, 1.f);


    return true;
}

void CColliderCube::Update(float DeltaTime)
{
    CCollider::Update(DeltaTime);
}

void CColliderCube::PostUpdate(float DeltaTime)
{
    CCollider::PostUpdate(DeltaTime);

    CaculateVertexPos();
    VerticalToFowardVector();
}
    

void CColliderCube::Collision(float DeltaTime)
{
    CCollider::Collision(DeltaTime);
}

void CColliderCube::PrevRender(float DeltaTime)
{
    CCollider::PrevRender(DeltaTime);
}

void CColliderCube::Render(float DeltaTime)
{
    CCollider::Render(DeltaTime);
}

CColliderCube* CColliderCube::Clone()
{
    return new CColliderCube(*this);
}

bool CColliderCube::Collision(CCollider* Dest)
{
    bool   result = false;

    switch (Dest->GetColliderShape())
    {
	case Collider_Shape::Cube:
	{
		HitResult   DestResult = {};
		result = CCollision::CollisionCubeToCube(m_HitResult, DestResult,
			this, (CColliderCube*)Dest);
		Dest->SetHitResult(DestResult);
	}
	break;
	}

	return result;
}

bool CColliderCube::CollisionMouse(const Vector2& MousePos)
{
    /*m_MouseCollision = CCollision::CollisionSpherePicking(m_HitResult, this,
       CInput::GetInst()->GetRay());*/

    return m_MouseCollision;
}

void CColliderCube::VerticalToFowardVector()
{
    Vector3 Line[4] = { Vector3(0.f,0.f,0.f) };

    LinePoint linePoint[4]; 
    linePoint[0].Point[0] = m_BottomPoint[0];
    linePoint[0].Point[1] = m_BottomPoint[1];

    linePoint[1].Point[0] = m_BottomPoint[1];
    linePoint[1].Point[1] = m_BottomPoint[2];

    linePoint[2].Point[0] = m_BottomPoint[2];
    linePoint[2].Point[1] = m_BottomPoint[3];

    linePoint[3].Point[0] = m_BottomPoint[3];
    linePoint[3].Point[1] = m_BottomPoint[0];

    Line[0] = m_BottomPoint[0] - m_BottomPoint[1];
    Line[1] = m_BottomPoint[1] - m_BottomPoint[2];
    Line[2] = m_BottomPoint[2] - m_BottomPoint[3];
    Line[3] = m_BottomPoint[3] - m_BottomPoint[0];

    std::vector<LinePoint> Vertical;

    for (int i = 0; i < 4; ++i)
    {
        Vector3 Line = linePoint[i].Point[0] - linePoint[i].Point[1];
        float Test = m_Info.Axis[AXIS_Z].Dot(Line);

        if (Test == 0.f)
        {
            if (Line.x > 0.f)
            {
                VerticalLineToFoward = linePoint[i];
            }
        }
    }
}

std::vector<Vector2> CColliderCube::GetNormalVectorToLine(const Vector2& pointStart, const Vector2& pointEnd)
{
    std::vector<Vector2> normalVector;

    Vector2 Line = pointEnd - pointStart;

    Vector2 normal1 = {-Line.y, Line.x};
    Vector2 normal2 = { Line.y, -Line.x };

    normalVector.push_back(normal1);
    normalVector.push_back(normal2);

    return normalVector;
}

void CColliderCube::CaculateVertexPos()
{
    m_Info.Center.x = GetWorldPos().x;
    m_Info.Center.y = GetWorldPos().y;
    m_Info.Center.z = GetWorldPos().z;


    m_Info.Axis[AXIS_X].x = GetAxis(AXIS_X).x;
    m_Info.Axis[AXIS_X].y = GetAxis(AXIS_X).y;
    m_Info.Axis[AXIS_X].z = GetAxis(AXIS_X).z;


    m_Info.Axis[AXIS_Y].x = GetAxis(AXIS_Y).x;
    m_Info.Axis[AXIS_Y].y = GetAxis(AXIS_Y).y;
    m_Info.Axis[AXIS_Y].z = GetAxis(AXIS_Y).z;

    m_Info.Axis[AXIS_Z].x = GetAxis(AXIS_Z).x;
    m_Info.Axis[AXIS_Z].y = GetAxis(AXIS_Z).y;
    m_Info.Axis[AXIS_Z].z = GetAxis(AXIS_Z).z;

    Vector3   Pos[8] = {};

    // 전면 4개의 점 반시계 방향
    Pos[0] = m_Info.Center + m_Info.Axis[AXIS_X] * m_Info.Length[0]
        + m_Info.Axis[AXIS_Y] * m_Info.Length[1]
        - m_Info.Axis[AXIS_Z] * m_Info.Length[2];
    Pos[1] = m_Info.Center - m_Info.Axis[AXIS_X] * m_Info.Length[0]
        + m_Info.Axis[AXIS_Y] * m_Info.Length[1]
        - m_Info.Axis[AXIS_Z] * m_Info.Length[2];
    Pos[2] = m_Info.Center - m_Info.Axis[AXIS_X] * m_Info.Length[0]
        - m_Info.Axis[AXIS_Y] * m_Info.Length[1]
        - m_Info.Axis[AXIS_Z] * m_Info.Length[2];
    Pos[3] = m_Info.Center + m_Info.Axis[AXIS_X] * m_Info.Length[0]
        - m_Info.Axis[AXIS_Y] * m_Info.Length[1]
        - m_Info.Axis[AXIS_Z] * m_Info.Length[2];

    Pos[4] = m_Info.Center + m_Info.Axis[AXIS_X] * m_Info.Length[0]
        + m_Info.Axis[AXIS_Y] * m_Info.Length[1]
        + m_Info.Axis[AXIS_Z] * m_Info.Length[2];
    Pos[5] = m_Info.Center - m_Info.Axis[AXIS_X] * m_Info.Length[0]
        + m_Info.Axis[AXIS_Y] * m_Info.Length[1]
        + m_Info.Axis[AXIS_Z] * m_Info.Length[2];
    Pos[6] = m_Info.Center - m_Info.Axis[AXIS_X] * m_Info.Length[0]
        - m_Info.Axis[AXIS_Y] * m_Info.Length[1]
        + m_Info.Axis[AXIS_Z] * m_Info.Length[2];
    Pos[7] = m_Info.Center + m_Info.Axis[AXIS_X] * m_Info.Length[0]
        - m_Info.Axis[AXIS_Y] * m_Info.Length[1]
        + m_Info.Axis[AXIS_Z] * m_Info.Length[2];

    // 사각형을 구성하는 8개의 점의 위치정보를 저장
    for (int i = 0; i < 8; ++i)
    {
        m_Info.PointPos[i] = Pos[i];
    }

    // 바닥면 저장 (회전 상황을 아직 고려하지 않음)
    m_BottomPoint[0] = Pos[7];
    m_BottomPoint[1] = Pos[6];
    m_BottomPoint[2] = Pos[2];
    m_BottomPoint[3] = Pos[3];

    m_Min.x = Pos[0].x;
    m_Min.y = Pos[0].y;
    m_Min.z = Pos[0].z;

    m_Max.x = Pos[0].x;
    m_Max.y = Pos[0].y;
    m_Max.z = Pos[0].z;


    for (int i = 1; i < 8; ++i)
    {
        if (m_Min.x > Pos[i].x)
            m_Min.x = Pos[i].x;

        if (m_Min.y > Pos[i].y)
            m_Min.y = Pos[i].y;

        if (m_Min.z > Pos[i].z)
            m_Min.z = Pos[i].z;

        if (m_Max.x < Pos[i].x)
            m_Max.x = Pos[i].x;

        if (m_Max.y < Pos[i].y)
            m_Max.y = Pos[i].y;

        if (m_Max.z < Pos[i].z)
            m_Max.z = Pos[i].z;
    }

    m_Info.Min = m_Min;
    m_Info.Max = m_Max;
}

bool CColliderCube::IsIntersectPlaneAndLine(Vector3 Line)
{
    
    return false;
}

