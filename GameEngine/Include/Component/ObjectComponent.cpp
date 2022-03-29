
#include "ObjectComponent.h"

CObjectComponent::CObjectComponent()
{
}

CObjectComponent::CObjectComponent(const CObjectComponent& com)	:
	CComponent(com)
{
	*this = com;
}

CObjectComponent::~CObjectComponent()
{
}

void CObjectComponent::Start()
{
}

bool CObjectComponent::Init()
{
	if (!CComponent::Init())
		return false;

	return true;
}

void CObjectComponent::Update(float DeltaTime)
{
	CComponent::Update(DeltaTime);
}

void CObjectComponent::PostUpdate(float DeltaTime)
{
	CComponent::PostUpdate(DeltaTime);
}

void CObjectComponent::Collision(float DeltaTime)
{
	CComponent::Collision(DeltaTime);
}

void CObjectComponent::PrevRender(float DeltaTime)
{
	CComponent::PrevRender(DeltaTime);
}

void CObjectComponent::Render(float DeltaTime)
{
	CComponent::Render(DeltaTime);
}

CObjectComponent* CObjectComponent::Clone()
{
	return new CObjectComponent(*this);
}
