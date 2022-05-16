
#include "Component.h"

CComponent::CComponent()
{
}

CComponent::CComponent(const CComponent& com)
{
}

CComponent::~CComponent()
{
}

void CComponent::Active(bool bActive)
{
    CRef::Active(bActive);
}

void CComponent::Start()
{
}

bool CComponent::Init()
{
    return true;
}

void CComponent::Update(float DeltaTime)
{
}

void CComponent::PostUpdate(float DeltaTime)
{
}

void CComponent::Collision(float DeltaTime)
{
}

void CComponent::PrevRender(float DeltaTime)
{
}

void CComponent::Render(float DeltaTime)
{
}
