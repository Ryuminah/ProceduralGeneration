
#include "DecalComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "Transform.h"
#include "../Resource/Material.h"
#include "../PathManager.h"
#include "../Resource/Texture.h"
#include "../Navigation3D.h"
#include "../NavigationManager.h"
#include "Camera.h"
#include "../Scene/CameraManager.h"
#include "../GameObject.h"

CDecalComponent::CDecalComponent()
{
	m_PrimitiveType = PrimitiveComponent_Type::Primitive3D;
	m_3DType = RT3D_Decal;

	SetTransformState(Transform_State::None);

	m_FadeInTime = 0.f;
	m_FadeIn = 0.f;

	m_FadeOutTime = 0.f;
	m_FadeOut = 0.f;

	m_DurationTime = 0.f;
	m_Duration = 0.f;

	m_FadeLoop = false;

	m_FadeState = Decal_Fade_State::Duration;
}

CDecalComponent::CDecalComponent(const CDecalComponent& com) :
	CPrimitiveComponent(com)
{
	m_pMesh = com.m_pMesh;
	m_pDebugMesh = com.m_pDebugMesh;

	m_FadeInTime = com.m_FadeInTime;
	m_FadeIn = com.m_FadeIn;

	m_FadeOutTime = com.m_FadeOutTime;
	m_FadeOut = com.m_FadeOut;

	m_DurationTime = com.m_DurationTime;
	m_Duration = com.m_Duration;

	m_FadeLoop = com.m_FadeLoop;

	m_FadeState = com.m_FadeState;
}

CDecalComponent::~CDecalComponent()
{
}

void CDecalComponent::Start()
{
	CPrimitiveComponent::Start();

	if (m_FadeInTime > 0.f)
	{
		m_FadeState = Decal_Fade_State::FadeIn;

		GetMaterial(0)->SetOpacity(0.f);
	}

	else
		m_FadeState = Decal_Fade_State::Duration;
}

bool CDecalComponent::Init()
{
	if (!CPrimitiveComponent::Init())
		return false;

	m_pDebugMesh = m_pScene->GetResource()->FindMesh("CubeLine");
	m_pMesh = m_pScene->GetResource()->FindMesh("Cube");

	CMaterial*	Mtrl = m_pScene->GetResource()->FindMaterial("DefaultDecal")->Clone();

	AddMaterial(Mtrl);

	SAFE_RELEASE(Mtrl);

	m_DebugMaterial = m_pScene->GetResource()->FindMaterial("DefaultDecalDebug")->Clone();

	m_DebugMaterial->Release();

	SetRelativeScale(3.f, 3.f, 3.f);

	return true;
}

void CDecalComponent::Update(float DeltaTime)
{
	CPrimitiveComponent::Update(DeltaTime);

	switch (m_FadeState)
	{
	case Decal_Fade_State::FadeIn:
		m_FadeIn += DeltaTime;

		GetMaterial(0)->SetOpacity(m_FadeIn / m_FadeInTime);

		if (m_FadeIn >= m_FadeInTime)
		{
			m_FadeState = Decal_Fade_State::Duration;

			GetMaterial(0)->SetOpacity(1.f);
		}
		break;
	case Decal_Fade_State::FadeOut:
		if (m_FadeOutTime > 0.f)
		{
			m_FadeOut += DeltaTime;

			GetMaterial(0)->SetOpacity(1.f - m_FadeOut / m_FadeOutTime);

			if (m_FadeOut >= m_FadeOutTime)
			{
				if (m_FadeLoop)
				{
					m_FadeIn = 0.f;
					m_FadeOut = 0.f;
					m_Duration = 0.f;

					if (m_FadeInTime > 0.f)
					{
						m_FadeState = Decal_Fade_State::FadeIn;
						GetMaterial(0)->SetOpacity(0.f);
					}

					else
					{
						m_FadeState = Decal_Fade_State::Duration;
						GetMaterial(0)->SetOpacity(1.f);
					}
				}

				else
					m_pOwner->Active(false);
			}
		}
		break;
	case Decal_Fade_State::Duration:
		if (m_DurationTime > 0.f)
		{
			m_Duration += DeltaTime;

			if (m_Duration >= m_DurationTime)
			{
				if (m_FadeOutTime > 0.f)
					m_FadeState = Decal_Fade_State::FadeOut;

				else if (m_FadeLoop)
				{
					m_Duration = 0.f;
					m_FadeIn = 0.f;
					m_FadeOut = 0.f;

					if (m_FadeInTime > 0.f)
					{
						m_FadeState = Decal_Fade_State::FadeIn;

						GetMaterial(0)->SetOpacity(0.f);
					}
				}

				else
					m_pOwner->Active(false);
			}
		}
		break;
	}
}

void CDecalComponent::PostUpdate(float DeltaTime)
{
	CPrimitiveComponent::PostUpdate(DeltaTime);
}

void CDecalComponent::Collision(float DeltaTime)
{
	CPrimitiveComponent::Collision(DeltaTime);
}

void CDecalComponent::Render(float DeltaTime)
{
	CPrimitiveComponent::Render(DeltaTime);

	for (int i = 0; i < (int)m_vecMaterialSlot.size(); ++i)
	{
		m_vecMaterialSlot[i]->SetMaterial();

		m_pMesh->Render(i);

		m_vecMaterialSlot[i]->ResetMaterial();
	}
}

void CDecalComponent::RenderDebug(float DeltaTime)
{
	CPrimitiveComponent::RenderDebug(DeltaTime);

	if (m_DebugMaterial && m_pDebugMesh)
	{
		m_DebugMaterial->SetMaterial();

		m_pDebugMesh->Render();

		m_DebugMaterial->ResetMaterial();
	}
}

CDecalComponent* CDecalComponent::Clone()
{
	return new CDecalComponent(*this);
}
