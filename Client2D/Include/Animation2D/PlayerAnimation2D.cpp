
#include "PlayerAnimation2D.h"
#include "Component/SpriteComponent.h"

CPlayerAnimation2D::CPlayerAnimation2D()
{
}

CPlayerAnimation2D::CPlayerAnimation2D(const CPlayerAnimation2D& anim)	:
	CAnimation2D(anim)
{
}

CPlayerAnimation2D::~CPlayerAnimation2D()
{
}

void CPlayerAnimation2D::Start()
{
	CAnimation2D::Start();
}

bool CPlayerAnimation2D::Init()
{
	if (!CAnimation2D::Init())
		return false;

	AddAnimationSequence2D("PlayerIdle");
	AddAnimationSequence2D("PlayerRun");
	AddAnimationSequence2D("PlayerAttack", false);

	return true;
}

void CPlayerAnimation2D::Update(float DeltaTime)
{
	CAnimation2D::Update(DeltaTime);

	if (m_Owner->GetVelocity().Length() > 0.f)
	{
		ChangeAnimation("PlayerRun");
	}

	else
	{
		if(GetCurrentSequenceName() != "PlayerAttack")
			ChangeAnimation("PlayerIdle");
	}
}

CPlayerAnimation2D* CPlayerAnimation2D::Clone()
{
	return new CPlayerAnimation2D(*this);
}
