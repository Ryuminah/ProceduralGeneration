
#include "PaperBurnComponent.h"
#include "../Resource/PaperBurnCBuffer.h"

CPaperBurnComponent::CPaperBurnComponent()
{
	m_CBuffer = new CPaperBurnCBuffer;

	m_CBuffer->Init();

	m_PaperBurnEnable = false;
	m_Inverse = false;

	m_BurnTimeMax = 1.f;
	m_BurnTime = 0.f;
}

CPaperBurnComponent::CPaperBurnComponent(const CPaperBurnComponent& com) :
	CObjectComponent(com)
{
	*this = com;

	m_CBuffer = com.m_CBuffer->Clone();
}

CPaperBurnComponent::~CPaperBurnComponent()
{
	SAFE_DELETE(m_CBuffer);

	size_t	Size = m_vecTargetMaterial.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecTargetMaterial[i]->DeleteSetMaterialCallback("PaperBurn");
		m_vecTargetMaterial[i]->DeleteResetMaterialCallback("PaperBurn");
	}
}

void CPaperBurnComponent::Start()
{
	CObjectComponent::Start();
}

bool CPaperBurnComponent::Init()
{
	if (!CObjectComponent::Init())
		return false;

	return true;
}

void CPaperBurnComponent::Update(float DeltaTime)
{
	CObjectComponent::Update(DeltaTime);

	if (m_PaperBurnEnable)
	{
		bool	End = false;

		m_BurnTime += DeltaTime;

		if (m_BurnTime >= m_BurnTimeMax)
		{
			End = true;
			m_BurnTime = m_BurnTimeMax;
		}

		m_CBuffer->SetBurn(m_BurnTime / m_BurnTimeMax);

		if (End && m_EndFunction)
			m_EndFunction();
	}

}

void CPaperBurnComponent::PostUpdate(float DeltaTime)
{
	CObjectComponent::PostUpdate(DeltaTime);
}

void CPaperBurnComponent::Collision(float DeltaTime)
{
	CObjectComponent::Collision(DeltaTime);
}

void CPaperBurnComponent::PrevRender(float DeltaTime)
{
	CObjectComponent::PrevRender(DeltaTime);
}

void CPaperBurnComponent::Render(float DeltaTime)
{
	CObjectComponent::Render(DeltaTime);
}

CPaperBurnComponent* CPaperBurnComponent::Clone()
{
	return new CPaperBurnComponent(*this);
}

void CPaperBurnComponent::SetInLineColor(const Vector3& Color)
{
	m_CBuffer->SetInLineColor(Color);
}

void CPaperBurnComponent::SetInLineColorUNorm(float r, float g, float b)
{
	m_CBuffer->SetInLineColorUNorm(r, g, b);
}

void CPaperBurnComponent::SetInLineColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_CBuffer->SetInLineColor(r, g, b);
}

void CPaperBurnComponent::SetCenterLineColor(const Vector3& Color)
{
	m_CBuffer->SetCenterColor(Color);
}

void CPaperBurnComponent::SetCenterLineColorUNorm(float r, float g, float b)
{
	m_CBuffer->SetCenterColorUNorm(r, g, b);
}

void CPaperBurnComponent::SetCenterLineColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_CBuffer->SetCenterColor(r, g, b);
}

void CPaperBurnComponent::SetOutLineColor(const Vector3& Color)
{
	m_CBuffer->SetOutLineColor(Color);
}

void CPaperBurnComponent::SetOutLineColorUNorm(float r, float g, float b)
{
	m_CBuffer->SetOutLineColorUNorm(r, g, b);
}

void CPaperBurnComponent::SetOutLineColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_CBuffer->SetOutLineColor(r, g, b);
}

void CPaperBurnComponent::SetInFilter(float Filter)
{
	m_CBuffer->SetInFilter(Filter);
}

void CPaperBurnComponent::SetCenterFilter(float Filter)
{
	m_CBuffer->SetCenterFilter(Filter);
}

void CPaperBurnComponent::SetOutFilter(float Filter)
{
	m_CBuffer->SetOutFilter(Filter);
}

void CPaperBurnComponent::Inverse()
{
	m_Inverse = true;

	m_CBuffer->SetBurnInverse(1);
}

void CPaperBurnComponent::SetMaterialCallback()
{
	if (!m_PaperBurnEnable)
		return;

	m_CBuffer->UpdateCBuffer();
}

void CPaperBurnComponent::ResetMaterialCallback()
{
	if (!m_PaperBurnEnable)
		return;
}
