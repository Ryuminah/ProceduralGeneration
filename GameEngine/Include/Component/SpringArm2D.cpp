
#include "SpringArm2D.h"
#include "../Scene/Scene.h"

CSpringArm2D::CSpringArm2D()
{
	m_SceneComponentType = SceneComponent_Type::SpringArm;
}

CSpringArm2D::CSpringArm2D(const CSpringArm2D& com) :
	CSceneComponent(com)
{
}

CSpringArm2D::~CSpringArm2D()
{
}

void CSpringArm2D::Start()
{
	CSceneComponent::Start();
}

bool CSpringArm2D::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CSpringArm2D::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CSpringArm2D::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	if (m_pParent)
	{
		Vector3	ParentPos = m_pParent->GetWorldPos();

		if (!GetInheritPosZ())
			ParentPos.z = 0.f;

		Vector3	Pos = ParentPos + m_Offset;

		SetWorldPos(Pos);
	}
}

void CSpringArm2D::Collision(float DeltaTime)
{
	CSceneComponent::Collision(DeltaTime);
}

void CSpringArm2D::PrevRender(float DeltaTime)
{
	CSceneComponent::PrevRender(DeltaTime);
}

void CSpringArm2D::Render(float DeltaTime)
{
	CSceneComponent::Render(DeltaTime);
}

CSpringArm2D* CSpringArm2D::Clone()
{
	return new CSpringArm2D(*this);
}
