
#include "DistortionComponent.h"
#include "../Resource/DistortionCBuffer.h"
#include "../Render/RenderManager.h"
#include "../Resource/RenderTarget.h"

CDistortionComponent::CDistortionComponent()
{
	m_CBuffer = new CDistortionCBuffer;

	m_CBuffer->Init();

	m_CBuffer->SetSelf(true);

	m_DistortionEnable = false;
}

CDistortionComponent::CDistortionComponent(const CDistortionComponent& com) :
	CObjectComponent(com)
{
	*this = com;

	m_CBuffer = com.m_CBuffer->Clone();
}

CDistortionComponent::~CDistortionComponent()
{
	SAFE_DELETE(m_CBuffer);

	size_t	Size = m_vecTargetMaterial.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecTargetMaterial[i]->DeleteSetMaterialCallback("Distortion");
		m_vecTargetMaterial[i]->DeleteResetMaterialCallback("Distortion");
	}
}

void CDistortionComponent::SetTargetPrimitive(CPrimitiveComponent* Primitive)
{
	m_TargetPrimitive = Primitive;

	m_TargetPrimitive->SetDistortion(m_DistortionEnable);
}

void CDistortionComponent::SetWeight(float Weight)
{
	m_CBuffer->SetWeight(Weight);
}

void CDistortionComponent::SetSpeed(float Speed)
{
	m_CBuffer->SetSpeed(Speed);
}

void CDistortionComponent::EnableDistortion(bool Enable)
{
	m_DistortionEnable = Enable;

	if (m_TargetPrimitive)
		m_TargetPrimitive->SetDistortion(m_DistortionEnable);

	size_t	Size = m_vecTargetMaterial.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecTargetMaterial[i]->DistortionEnable(Enable);
	}
}

void CDistortionComponent::Start()
{
	CObjectComponent::Start();
}

bool CDistortionComponent::Init()
{
	if (!CObjectComponent::Init())
		return false;

	return true;
}

void CDistortionComponent::Update(float DeltaTime)
{
	CObjectComponent::Update(DeltaTime);
}

void CDistortionComponent::PostUpdate(float DeltaTime)
{
	CObjectComponent::PostUpdate(DeltaTime);
}

void CDistortionComponent::Collision(float DeltaTime)
{
	CObjectComponent::Collision(DeltaTime);
}

void CDistortionComponent::PrevRender(float DeltaTime)
{
	CObjectComponent::PrevRender(DeltaTime);
}

void CDistortionComponent::Render(float DeltaTime)
{
	CObjectComponent::Render(DeltaTime);
}

CDistortionComponent* CDistortionComponent::Clone()
{
	return new CDistortionComponent(*this);
}

void CDistortionComponent::SetMaterialCallback()
{
	m_CBuffer->UpdateCBuffer();
}

void CDistortionComponent::ResetMaterialCallback()
{
}
