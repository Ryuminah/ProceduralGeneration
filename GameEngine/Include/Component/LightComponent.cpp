
#include "LightComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/LightManager.h"
#include "../Scene/CameraManager.h"
#include "Camera.h"

CLightComponent::CLightComponent()
{
	m_SceneComponentType = SceneComponent_Type::Light;
	m_CBuffer = nullptr;

	SetTransformState(Transform_State::None);
}

CLightComponent::CLightComponent(const CLightComponent& com) :
	CSceneComponent(com)
{
	m_CBuffer = com.m_CBuffer->Clone();
}

CLightComponent::~CLightComponent()
{
	SAFE_DELETE(m_CBuffer);
}

void CLightComponent::Start()
{
	CSceneComponent::Start();

	m_pScene->GetLightManager()->AddLight(this);
}

bool CLightComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	// ������� ����
	m_CBuffer = new CLightConstantBuffer;

	m_CBuffer->Init();

	return true;
}

void CLightComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CLightComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();

	if (m_CBuffer->GetLightType() != Light_Type::Dir)
	{
		// ���� �� ������ ��ġ�� ������������� ��ġ�̴�. �̸� �� �������� �ٲ㼭
		// �Ѱ��ش�.
		Vector3	Pos = GetWorldPos();

		Pos = Pos.TransformCoord(Camera->GetViewMatrix());

		m_CBuffer->SetPos(Pos);

		if (m_CBuffer->GetLightType() == Light_Type::Point)
			SetRelativeScale(m_CBuffer->GetLightDistance(), m_CBuffer->GetLightDistance(), m_CBuffer->GetLightDistance());
	}

	if (m_CBuffer->GetLightType() != Light_Type::Point)
	{
		Vector3	Dir = GetAxis(AXIS_Z);

		Dir = Dir.TransformNormal(Camera->GetViewMatrix());
		Dir.Normalize();

		m_CBuffer->SetDir(Dir);
	}
}

void CLightComponent::Collision(float DeltaTime)
{
	CSceneComponent::Collision(DeltaTime);
}

void CLightComponent::PrevRender(float DeltaTime)
{
	CSceneComponent::PrevRender(DeltaTime);
}

void CLightComponent::Render(float DeltaTime)
{
	CSceneComponent::Render(DeltaTime);
}

CLightComponent* CLightComponent::Clone()
{
	return new CLightComponent(*this);
}

void CLightComponent::SetShader()
{
	m_CBuffer->UpdateCBuffer();
}
