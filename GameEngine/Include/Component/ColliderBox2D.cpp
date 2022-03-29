
#include "ColliderBox2D.h"
#include "../Scene/Scene.h"
#include "../Collision.h"
#include "../Scene/SceneResource.h"
#include "ColliderCircle.h"
#include "ColliderPixel.h"

CColliderBox2D::CColliderBox2D()
{
	m_ColliderShape = Collider_Shape::Box2D;
	m_ColliderSpace = Collider_Space::Collider2D;

	m_PrimitiveType = PrimitiveComponent_Type::Primitive2D;
	m_2DType = RT2D_Default;
}

CColliderBox2D::CColliderBox2D(const CColliderBox2D& com) :
	CCollider(com)
{
	m_Info = com.m_Info;
}

CColliderBox2D::~CColliderBox2D()
{
}

void CColliderBox2D::Start()
{
	CCollider::Start();

	SetWorldScale(m_Info.Length[0] * 2.f, m_Info.Length[1] * 2.f, 1.f);

	m_DebugMesh = m_pScene->GetResource()->FindMesh("ColliderBox2D");
}

bool CColliderBox2D::Init()
{
	if (!CCollider::Init())
		return false;

	m_Info.Axis[AXIS_X] = Vector2(1.f, 0.f);
	m_Info.Axis[AXIS_Y] = Vector2(0.f, 1.f);
	
	SetInheritRotZ(true);

	SetUpdatePosZ(true);

	return true;
}

void CColliderBox2D::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CColliderBox2D::PostUpdate(float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);

	m_Info.Center.x = GetWorldPos().x;
	m_Info.Center.y = GetWorldPos().y;

	m_Info.Axis[AXIS_X].x = GetAxis(AXIS_X).x;
	m_Info.Axis[AXIS_X].y = GetAxis(AXIS_X).y;

	m_Info.Axis[AXIS_Y].x = GetAxis(AXIS_Y).x;
	m_Info.Axis[AXIS_Y].y = GetAxis(AXIS_Y).y;

	Vector2	Pos[4] = {};

	Pos[0] = m_Info.Center - m_Info.Axis[AXIS_X] * m_Info.Length[0] +
		m_Info.Axis[AXIS_Y] * m_Info.Length[1];
	Pos[1] = m_Info.Center + m_Info.Axis[AXIS_X] * m_Info.Length[0] +
		m_Info.Axis[AXIS_Y] * m_Info.Length[1];

	Pos[2] = m_Info.Center - m_Info.Axis[AXIS_X] * m_Info.Length[0] -
		m_Info.Axis[AXIS_Y] * m_Info.Length[1];
	Pos[3] = m_Info.Center + m_Info.Axis[AXIS_X] * m_Info.Length[0] -
		m_Info.Axis[AXIS_Y] * m_Info.Length[1];

	m_Min.x = Pos[0].x;
	m_Min.y = Pos[0].y;

	m_Max.x = Pos[0].x;
	m_Max.y = Pos[0].y;

	for (int i = 1; i < 4; ++i)
	{
		if (m_Min.x > Pos[i].x)
			m_Min.x = Pos[i].x;

		if (m_Min.y > Pos[i].y)
			m_Min.y = Pos[i].y;

		if (m_Max.x < Pos[i].x)
			m_Max.x = Pos[i].x;

		if (m_Max.y < Pos[i].y)
			m_Max.y = Pos[i].y;
	}

	m_Info.Min = m_Min;
	m_Info.Max = m_Max;
}

void CColliderBox2D::Collision(float DeltaTime)
{
	CCollider::Collision(DeltaTime);
}

void CColliderBox2D::PrevRender(float DeltaTime)
{
	CCollider::PrevRender(DeltaTime);
}

void CColliderBox2D::Render(float DeltaTime)
{
	CCollider::Render(DeltaTime);
}

CColliderBox2D* CColliderBox2D::Clone()
{
	return new CColliderBox2D(*this);
}

bool CColliderBox2D::Collision(CCollider* Dest)
{
	bool	result = false;

	switch (Dest->GetColliderShape())
	{
	case Collider_Shape::Box2D:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionBox2DToBox2D(m_HitResult, DestResult,
			this, (CColliderBox2D*)Dest);
		Dest->SetHitResult(DestResult);
	}
		break;
	case Collider_Shape::Circle:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionBox2DToCircle(m_HitResult, DestResult,
			this, (CColliderCircle*)Dest);
		Dest->SetHitResult(DestResult);
	}
		break;
	case Collider_Shape::Pixel:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionBox2DToPixel(m_HitResult, DestResult,
			this, (CColliderPixel*)Dest);
		Dest->SetHitResult(DestResult);
	}
	break;
	}

	return result;
}

bool CColliderBox2D::CollisionMouse(const Vector2& MousePos)
{	
	HitResult	SrcResult = {};
	HitResult	DestResult = {};

	m_MouseCollision = CCollision::CollisionBox2DToPoint(SrcResult, DestResult,
		m_Info, MousePos);

	return m_MouseCollision;
}
