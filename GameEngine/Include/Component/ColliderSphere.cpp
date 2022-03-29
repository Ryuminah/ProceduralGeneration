
#include "ColliderSphere.h"
#include "../Scene/Scene.h"
#include "../Collision.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"
#include "../Render/RenderStateManager.h"

CColliderSphere::CColliderSphere()
{
	m_ColliderShape = Collider_Shape::Sphere;
	m_ColliderSpace = Collider_Space::Collider3D;

	m_PrimitiveType = PrimitiveComponent_Type::Primitive3D;
	m_3DType = RT3D_Default;

	m_Info.Radius = 1.f;
}

CColliderSphere::CColliderSphere(const CColliderSphere& com) :
	CCollider(com)
{
	m_Info = com.m_Info;
}

CColliderSphere::~CColliderSphere()
{
}

void CColliderSphere::Start()
{
	CCollider::Start();

	SetWorldScale(m_Info.Radius, m_Info.Radius, m_Info.Radius);

	m_DebugMesh = m_pScene->GetResource()->FindMesh("SpherePos");
}

bool CColliderSphere::Init()
{
	if (!CCollider::Init())
		return false;

	//SetUpdatePosZ(true);
	m_WireFrame = CRenderStateManager::GetInst()->FindRenderState("WireFrame");
	m_DebugShader = CShaderManager::GetInst()->FindShader("Collider3DShader");

	return true;
}

void CColliderSphere::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CColliderSphere::PostUpdate(float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);

	m_Info.Center = GetWorldPos() + m_Offset;

	m_Min = m_Info.Center - m_Info.Radius;
	m_Max = m_Info.Center + m_Info.Radius;

	m_Info.Min = m_Min;
	m_Info.Max = m_Max;
}

void CColliderSphere::Collision(float DeltaTime)
{
	CCollider::Collision(DeltaTime);
}

void CColliderSphere::PrevRender(float DeltaTime)
{
	CCollider::PrevRender(DeltaTime);
}

void CColliderSphere::Render(float DeltaTime)
{
	CCollider::Render(DeltaTime);
}

CColliderSphere* CColliderSphere::Clone()
{
	return new CColliderSphere(*this);
}

bool CColliderSphere::Collision(CCollider* Dest)
{
	bool	result = false;

	switch (Dest->GetColliderShape())
	{
	case Collider_Shape::Sphere:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionSphereToSphere(m_HitResult, DestResult,
			this, (CColliderSphere*)Dest);
		Dest->SetHitResult(DestResult);
	}
	break;
	}

	return result;
}

bool CColliderSphere::CollisionMouse(const Vector2& MousePos)
{
	m_MouseCollision = CCollision::CollisionSpherePicking(m_HitResult, this,
		CInput::GetInst()->GetRay());

	return m_MouseCollision;
}
