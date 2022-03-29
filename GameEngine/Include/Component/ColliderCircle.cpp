
#include "ColliderCircle.h"
#include "../Scene/Scene.h"
#include "../Collision.h"
#include "../Scene/SceneResource.h"
#include "ColliderBox2D.h"
#include "ColliderPixel.h"

CColliderCircle::CColliderCircle()
{
	m_ColliderShape = Collider_Shape::Circle;
	m_ColliderSpace = Collider_Space::Collider2D;

	m_PrimitiveType = PrimitiveComponent_Type::Primitive2D;
	m_2DType = RT2D_Default;

	m_Info.Radius = 50.f;
}

CColliderCircle::CColliderCircle(const CColliderCircle& com) :
	CCollider(com)
{
	m_Info = com.m_Info;
}

CColliderCircle::~CColliderCircle()
{
}

void CColliderCircle::Start()
{
	CCollider::Start();

	SetWorldScale(m_Info.Radius, m_Info.Radius, 1.f);

	m_DebugMesh = m_pScene->GetResource()->FindMesh("ColliderCircle");
}

bool CColliderCircle::Init()
{
	if (!CCollider::Init())
		return false;

	SetUpdatePosZ(true);

	return true;
}

void CColliderCircle::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CColliderCircle::PostUpdate(float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);

	m_Info.Center.x = GetWorldPos().x;
	m_Info.Center.y = GetWorldPos().y;

	Vector2	Pos[4] = {};

	Pos[0] = m_Info.Center - Vector2(1.f, 0.f) * m_Info.Radius +
		Vector2(0.f, 1.f) * m_Info.Radius;
	Pos[1] = m_Info.Center + Vector2(1.f, 0.f) * m_Info.Radius +
		Vector2(0.f, 1.f) * m_Info.Radius;

	Pos[2] = m_Info.Center - Vector2(1.f, 0.f) * m_Info.Radius -
		Vector2(0.f, 1.f) * m_Info.Radius;
	Pos[3] = m_Info.Center + Vector2(1.f, 0.f) * m_Info.Radius -
		Vector2(0.f, 1.f) * m_Info.Radius;

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

void CColliderCircle::Collision(float DeltaTime)
{
	CCollider::Collision(DeltaTime);
}

void CColliderCircle::PrevRender(float DeltaTime)
{
	CCollider::PrevRender(DeltaTime);
}

void CColliderCircle::Render(float DeltaTime)
{
	CCollider::Render(DeltaTime);
}

CColliderCircle* CColliderCircle::Clone()
{
	return new CColliderCircle(*this);
}

bool CColliderCircle::Collision(CCollider* Dest)
{
	bool	result = false;

	switch (Dest->GetColliderShape())
	{
	case Collider_Shape::Box2D:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionBox2DToCircle(DestResult, m_HitResult,
			(CColliderBox2D*)Dest, this);
		Dest->SetHitResult(DestResult);
	}
		break;
	case Collider_Shape::Circle:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionCircleToCircle(m_HitResult, DestResult,
			this, (CColliderCircle*)Dest);
		Dest->SetHitResult(DestResult);
	}
		break;
	case Collider_Shape::Pixel:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionCircleToPixel(m_HitResult, DestResult,
			this, (CColliderPixel*)Dest);
		Dest->SetHitResult(DestResult);
	}
	break;
	}

	return result;
}

bool CColliderCircle::CollisionMouse(const Vector2& MousePos)
{
	HitResult	SrcResult = {};
	HitResult	DestResult = {};

	m_MouseCollision = CCollision::CollisionCircleToPoint(SrcResult, DestResult,
		m_Info, MousePos);

	return m_MouseCollision;
}
