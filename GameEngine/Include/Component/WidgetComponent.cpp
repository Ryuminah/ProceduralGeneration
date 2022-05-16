
#include "WidgetComponent.h"
#include "Camera.h"
#include "../Scene/CameraManager.h"
#include "../Render/RenderStateManager.h"
#include "../Render/RenderState.h"

CWidgetComponent::CWidgetComponent()
{
	m_PrimitiveType = PrimitiveComponent_Type::PrimitiveWidget;
	m_2DType = RT2D_Default;
	m_3DType = RT3D_Default;
	m_Space = WidgetComponent_Space::Space2D;

	m_AlphaBlend = nullptr;
}

CWidgetComponent::CWidgetComponent(const CWidgetComponent& com) :
	CPrimitiveComponent(com)
{
	m_Space = com.m_Space;

	if (com.m_WidgetWindow)
		m_WidgetWindow = com.m_WidgetWindow->Clone();

	m_AlphaBlend = com.m_AlphaBlend;
}

CWidgetComponent::~CWidgetComponent()
{
}

void CWidgetComponent::Start()
{
	CPrimitiveComponent::Start();
}

bool CWidgetComponent::Init()
{
	if (!CPrimitiveComponent::Init())
		return false;

	m_AlphaBlend = CRenderStateManager::GetInst()->FindRenderState("AlphaBlend");

	SetUpdatePosZ(true);

	SetInheritRotX(false);
	SetInheritRotY(false);
	SetInheritRotZ(false);

	return true;
}

void CWidgetComponent::Update(float DeltaTime)
{
	CPrimitiveComponent::Update(DeltaTime);

	if (m_WidgetWindow)
	{
		m_WidgetWindow->Update(DeltaTime);
	}
}

void CWidgetComponent::PostUpdate(float DeltaTime)
{
	CPrimitiveComponent::PostUpdate(DeltaTime);

	if (m_WidgetWindow)
	{
		if (m_Space == WidgetComponent_Space::Space2D)
		{
			Vector2	WidgetPos = Vector2(GetWorldPos().x, GetWorldPos().y);

			// 2D 에서는 카메라의 위치를 빼서 윈도우 상에서의 위치를 구해준다.
			CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();

			Vector2	CameraPos = Vector2(Camera->GetWorldPos().x, Camera->GetWorldPos().y);

			WidgetPos -= CameraPos;

			m_WidgetWindow->SetPos(WidgetPos);
		}

		else
		{
		}

		m_WidgetWindow->PostUpdate(DeltaTime);
	}
}

void CWidgetComponent::Collision(float DeltaTime)
{
	CPrimitiveComponent::Collision(DeltaTime);
}

void CWidgetComponent::Render(float DeltaTime)
{
	CPrimitiveComponent::Render(DeltaTime);

	m_AlphaBlend->SetState();

	m_WidgetWindow->Render();

	m_AlphaBlend->ResetState();
}

CWidgetComponent* CWidgetComponent::Clone()
{
	return new CWidgetComponent(*this);
}
