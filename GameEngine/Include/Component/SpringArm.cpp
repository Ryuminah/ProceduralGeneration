
#include "SpringArm.h"
#include "../Scene/Scene.h"

CSpringArm::CSpringArm()
{
	m_SceneComponentType = SceneComponent_Type::SpringArm;

	m_TargetLength = 300.f;
}

CSpringArm::CSpringArm(const CSpringArm& com) :
	CSceneComponent(com)
{
}

CSpringArm::~CSpringArm()
{
}

void CSpringArm::Start()
{
	CSceneComponent::Start();
}

bool CSpringArm::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CSpringArm::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CSpringArm::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	if (m_pParent)
	{
		Vector3	Pos = (m_pParent->GetWorldPos() + m_Offset) - GetAxis(AXIS_Z) * m_TargetLength;

		SetWorldPos(Pos);
	}
}

void CSpringArm::Collision(float DeltaTime)
{
	CSceneComponent::Collision(DeltaTime);
}

void CSpringArm::PrevRender(float DeltaTime)
{
	CSceneComponent::PrevRender(DeltaTime);
}

void CSpringArm::Render(float DeltaTime)
{
	CSceneComponent::Render(DeltaTime);
}

CSpringArm* CSpringArm::Clone()
{
	return new CSpringArm(*this);
}
