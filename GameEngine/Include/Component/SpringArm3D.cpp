
#include "SpringArm3D.h"
#include "../Scene/Scene.h"

CSpringArm3D::CSpringArm3D()
{
	m_SceneComponentType = SceneComponent_Type::SpringArm;
	m_Length = 5.f;
}

CSpringArm3D::CSpringArm3D(const CSpringArm3D& com) :
	CSceneComponent(com)
{
	m_Length = com.m_Length;
}

CSpringArm3D::~CSpringArm3D()
{
}

void CSpringArm3D::Start()
{
	CSceneComponent::Start();
}

bool CSpringArm3D::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CSpringArm3D::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CSpringArm3D::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	if (m_pParent)
	{
		Vector3	ParentPos = m_pParent->GetWorldPos();

		Vector3	Pos = ParentPos + GetAxis(AXIS_Z) * -m_Length + m_Offset;

		SetWorldPos(Pos);
	}
}

void CSpringArm3D::Collision(float DeltaTime)
{
	CSceneComponent::Collision(DeltaTime);
}

void CSpringArm3D::PrevRender(float DeltaTime)
{
	CSceneComponent::PrevRender(DeltaTime);
}

void CSpringArm3D::Render(float DeltaTime)
{
	CSceneComponent::Render(DeltaTime);
}

CSpringArm3D* CSpringArm3D::Clone()
{
	return new CSpringArm3D(*this);
}
