
#include "LightManager.h"
#include "../Component/LightComponent.h"
#include "../GameObject.h"
#include "Scene.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Shader.h"
#include "../Device.h"
#include "SceneResource.h"
#include "../Render/RenderState.h"
#include "../Render/RenderStateManager.h"

CLightManager::CLightManager()
{
}

CLightManager::~CLightManager()
{
}

void CLightManager::AddLight(CLightComponent* Light)
{
	m_LightList.push_back(Light);
}

bool CLightManager::Init()
{
	// 전역조명 생성
	m_GlobalLight = m_pScene->SpawnObject<CGameObject>("GlobalLight", Vector3(0.f, 3.f, 0.f),
		Vector3(0.f, 0.f, 45.f));

	m_GlobalLightComponent = m_GlobalLight->CreateSceneComponent<CLightComponent>("GlobalLight");

	m_GlobalLight->SetRootComponent(m_GlobalLightComponent);

	m_GlobalLightComponent->SetRelativeRotation(45.f, 90.f, 0.f);

	//Com->SetLightType(Light_Type::Point);
	//Com->SetRelativePos(0.f, 1.5f, 0.f);
	//Com->SetDistance(2.5f);

	m_LightDirShader = CShaderManager::GetInst()->FindShader("LightDirAccShader");
	m_LightShader = CShaderManager::GetInst()->FindShader("LightAccShader");

	m_SphereMesh = m_pScene->GetResource()->FindMesh("SpherePos");

	m_DepthWriteDisable = CRenderStateManager::GetInst()->FindRenderState("DepthWriteDisable");
	m_LightVolumeBackState = CRenderStateManager::GetInst()->FindRenderState("LightVolumeBackState");
	m_LightVolumeFrontState = CRenderStateManager::GetInst()->FindRenderState("LightVolumeFrontState");
	m_FrontFaceCull = CRenderStateManager::GetInst()->FindRenderState("FrontFaceCulling");
	m_CullNone = CRenderStateManager::GetInst()->FindRenderState("CullNone");
	m_LightAccState = CRenderStateManager::GetInst()->FindRenderState("LightAcc");

	//Com->SetLightType(Light_Type::Point);
	//Com->SetDistance(5.f);

	return true;
}

void CLightManager::Update(float DeltaTime)
{
	auto	iter = m_LightList.begin();
	auto	iterEnd = m_LightList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_LightList.erase(iter);
			iterEnd = m_LightList.end();
			continue;
		}

		++iter;
	}
}

void CLightManager::SetShader()
{
	auto	iter = m_LightList.begin();
	auto	iterEnd = m_LightList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
			continue;

		(*iter)->SetShader();
	}
}

void CLightManager::Render()
{
	auto	iter = m_LightList.begin();
	auto	iterEnd = m_LightList.end();

	m_LightAccState->SetState();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
			continue;

		(*iter)->SetShader();

		// Transform을 Shader로 넘겨준다.
		(*iter)->Render(0.f);

		switch ((*iter)->GetLightType())
		{
		case Light_Type::Dir:
		{
			m_LightDirShader->SetShader();
			m_DepthWriteDisable->SetState();

			//CONTEXT->IASetInputLayout(nullptr);

			UINT Offset = 0;
			CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			CONTEXT->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
			CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

			CONTEXT->Draw(4, 0);

			m_DepthWriteDisable->ResetState();
		}
			break;
		case Light_Type::Point:
			m_LightShader->SetShader();

			m_FrontFaceCull->SetState();
			m_LightVolumeBackState->SetState();

			m_SphereMesh->Render();

			m_LightVolumeBackState->ResetState();
			m_FrontFaceCull->ResetState();

			m_CullNone->SetState();
			m_LightVolumeFrontState->SetState();

			m_SphereMesh->Render();

			m_CullNone->ResetState();
			m_LightVolumeFrontState->ResetState();
			break;
		case Light_Type::Spot:
			m_LightShader->SetShader();
			break;
		}
	}

	m_LightAccState->ResetState();
}
