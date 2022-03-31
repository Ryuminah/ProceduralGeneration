
#include "RenderManager.h"
#include "../Component/PrimitiveComponent.h"
#include "RenderStateManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Viewport.h"
#include "../Scene/CameraManager.h"
#include "../Scene/LightManager.h"
#include "../Component/Camera.h"
#include "../Device.h"
#include "RenderState.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/PostProcessCBuffer.h"
#include "../Resource/DistortionCBuffer.h"
#include "../Resource/LightAccShader.h"
#include "../Resource/ShadowCBuffer.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()	:
	m_PostProcessCBuffer(nullptr),
	m_DistortionCBuffer(nullptr),
	m_ShadowDepth(nullptr),
	m_ShadowCBuffer(nullptr),
	m_ShadowBias(0.00032f),
	m_ShadowMapSize(4096)
{
	m_RenderSpace = Render_Space::Render2D;

	for (int i = 0; i < RT2D_End; ++i)
	{
		m_RenderList2DCount[i] = 0;
		m_RenderList2DCapacity[i] = 0;
		m_pRenderList2D[i] = nullptr;
	}

	for (int i = 0; i < RT3D_End; ++i)
	{
		m_RenderList3DCount[i] = 0;
		m_RenderList3DCapacity[i] = 0;
		m_pRenderList3D[i] = nullptr;
	}

	m_RenderListDistortionCount = 0;
	m_RenderListDistortionCapacity = 0;
	m_pRenderListDistortion = nullptr;

	memset(m_WorldState, 0, sizeof(CRenderState*) * RST_End);
}

CRenderManager::~CRenderManager()
{
	SAFE_DELETE(m_ShadowCBuffer);
	SAFE_DELETE(m_PostProcessCBuffer);
	SAFE_DELETE(m_DistortionCBuffer);

	SAFE_DELETE_ARRAY(m_pRenderListDistortion);

	for (int i = 0; i < RT2D_End; ++i)
	{
		SAFE_DELETE_ARRAY(m_pRenderList2D[i]);
	}

	for (int i = 0; i < RT3D_End; ++i)
	{
		SAFE_DELETE_ARRAY(m_pRenderList3D[i]);
	}

	SAFE_RELEASE(m_ShadowDepth);

	CRenderStateManager::DestroyInst();
}

void CRenderManager::SetWorldRenderState(const std::string& Name)
{
	CRenderState* State = CRenderStateManager::GetInst()->FindRenderState(Name);

	if (!State)
		return;

	m_WorldState[State->GetType()] = State;
}

void CRenderManager::SetBlur(bool Enable)
{
	m_PostProcessCBuffer->SetBlur(Enable);
}

void CRenderManager::SetDistortion(bool Enable)
{
	m_PostProcessCBuffer->SetDistortion(Enable);
}

void CRenderManager::SetDistortionWeight(float Weight)
{
	m_DistortionCBuffer->SetWeight(Weight);
}

void CRenderManager::SetDistortionSpeed(float Speed)
{
	m_DistortionCBuffer->SetSpeed(Speed);
}

bool CRenderManager::Init()
{
	// 렌더링 상태 관리자 초기화
	if (!CRenderStateManager::GetInst()->Init())
		return false;

	m_DepthDisable = CRenderStateManager::GetInst()->FindRenderState("DepthDisable");
	m_DepthWriteDisable = CRenderStateManager::GetInst()->FindRenderState("DepthWriteDisable");
	m_AlphaBlend = CRenderStateManager::GetInst()->FindRenderState("AlphaBlend");
	m_UIAlphaBlend = CRenderStateManager::GetInst()->FindRenderState("UIAlphaBlend");


	m_SkyState = CRenderStateManager::GetInst()->FindRenderState("SkyState");
	m_CullNone = CRenderStateManager::GetInst()->FindRenderState("CullNone");

	m_DecalBlend = CRenderStateManager::GetInst()->FindRenderState("DecalBlend");

	return true;
}

void CRenderManager::SetDefaultTarget()
{
	m_DiffuseTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("DiffuseTarget");
	m_DistortionTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("DistortionTarget");

	Resolution	RS = CDevice::GetInst()->GetResolution();

	CResourceManager::GetInst()->CreateTarget("AlbedoTarget", RS.Width, RS.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM);
	CResourceManager::GetInst()->CreateTarget("GBuffer1", RS.Width, RS.Height,
		DXGI_FORMAT_R32G32B32A32_FLOAT);
	CResourceManager::GetInst()->CreateTarget("GBuffer2", RS.Width, RS.Height,
		DXGI_FORMAT_R32G32B32A32_FLOAT);
	CResourceManager::GetInst()->CreateTarget("GBuffer3", RS.Width, RS.Height,
		DXGI_FORMAT_R32G32B32A32_FLOAT);
	CResourceManager::GetInst()->CreateTarget("GBuffer4", RS.Width, RS.Height,
		DXGI_FORMAT_R32G32B32A32_FLOAT);
	CResourceManager::GetInst()->CreateTarget("GBuffer5", RS.Width, RS.Height,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	CRenderTarget* GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("AlbedoTarget");
	GBufferTarget->SetPos(0.f, 0.f, 0.f);
	GBufferTarget->SetScale(100.f, 100.f, 1.f);
	m_vecGBuffer.push_back(GBufferTarget);
	m_vecDecal.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer1");
	GBufferTarget->SetPos(0.f, 100.f, 0.f);
	GBufferTarget->SetScale(100.f, 100.f, 1.f);
	m_vecGBuffer.push_back(GBufferTarget);
	m_vecDecal.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer2");
	GBufferTarget->SetPos(0.f, 200.f, 0.f);
	GBufferTarget->SetScale(100.f, 100.f, 1.f);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer3");
	GBufferTarget->SetPos(0.f, 300.f, 0.f);
	GBufferTarget->SetScale(100.f, 100.f, 1.f);
	m_vecGBuffer.push_back(GBufferTarget);
	m_vecDecal.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer4");
	GBufferTarget->SetPos(0.f, 400.f, 0.f);
	GBufferTarget->SetScale(100.f, 100.f, 1.f);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer5");
	GBufferTarget->SetPos(0.f, 500.f, 0.f);
	GBufferTarget->SetScale(100.f, 100.f, 1.f);
	m_vecGBuffer.push_back(GBufferTarget);

	for (size_t i = 0; i < m_vecGBuffer.size(); ++i)
	{
		m_vecGBuffer[i]->SetDebugRender(true);
	}

	CResourceManager::GetInst()->CreateTarget("LightDif", RS.Width, RS.Height,
		DXGI_FORMAT_R32G32B32A32_FLOAT);
	CResourceManager::GetInst()->CreateTarget("LightSpc", RS.Width, RS.Height,
		DXGI_FORMAT_R32G32B32A32_FLOAT);
	CResourceManager::GetInst()->CreateTarget("LightEmv", RS.Width, RS.Height,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	CRenderTarget* LightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightDif");
	LightTarget->SetPos(100.f, 0.f, 0.f);
	LightTarget->SetScale(100.f, 100.f, 1.f);
	m_vecLight.push_back(LightTarget);

	LightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightSpc");
	LightTarget->SetPos(100.f, 100.f, 0.f);
	LightTarget->SetScale(100.f, 100.f, 1.f);
	m_vecLight.push_back(LightTarget);

	LightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightEmv");
	LightTarget->SetPos(100.f, 200.f, 0.f);
	LightTarget->SetScale(100.f, 100.f, 1.f);
	m_vecLight.push_back(LightTarget);

	for (size_t i = 0; i < m_vecLight.size(); ++i)
	{
		m_vecLight[i]->SetDebugRender(true);
	}

	CResourceManager::GetInst()->CreateTarget("LightCombine", RS.Width, RS.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM);

	m_LightCombineTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightCombine");
	m_LightCombineTarget->SetPos(200.f, 0.f, 0.f);
	m_LightCombineTarget->SetScale(100.f, 100.f, 1.f);
	m_LightCombineTarget->SetDebugRender(true);


	CResourceManager::GetInst()->CreateTarget("Shadow", m_ShadowMapSize, m_ShadowMapSize,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	m_ShadowTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("Shadow");
	m_ShadowTarget->SetPos(300.f, 0.f, 0.f);
	m_ShadowTarget->SetScale(500.f, 500.f, 1.f);
	m_ShadowTarget->SetDebugRender(true);

	// 그림자 그리기용 상수버퍼
	m_ShadowCBuffer = new CShadowCBuffer;

	m_ShadowCBuffer->Init();


	CDevice::GetInst()->CreateWorld2DTarget(m_DiffuseTarget->GetSurface());

	CResourceManager::GetInst()->CreateMaterial("DiffuseTargetMtrl");
	CResourceManager::GetInst()->AddMaterialTexture("DiffuseTargetMtrl",
		"DiffuseTarget");
	CResourceManager::GetInst()->SetMaterialShader("DiffuseTargetMtrl",
		"FullScreenDistortionShader");

	CResourceManager::GetInst()->CreateMaterial("DistortionTargetMtrl");
	CResourceManager::GetInst()->AddMaterialTexture("DistortionTargetMtrl",
		"DistortionTarget");
	CResourceManager::GetInst()->SetMaterialShader("DistortionTargetMtrl",
		"FullScreenShader");

	m_DiffuseTargetMaterial = CResourceManager::GetInst()->FindMaterial("DiffuseTargetMtrl");

	m_DistortionTargetMaterial = CResourceManager::GetInst()->FindMaterial("DistortionTargetMtrl");

	m_PostProcessCBuffer = new CPostProcessCBuffer;

	m_PostProcessCBuffer->Init();

	m_DistortionCBuffer = new CDistortionCBuffer;

	m_DistortionCBuffer->Init();

	m_DistortionCBuffer->SetWeight(0.05f);

	SetBlur(false);
	SetDistortion(false);


	m_LightAccShader = CShaderManager::GetInst()->FindShader("LightAccShader");
	m_LightCombineShader = CShaderManager::GetInst()->FindShader("LightCombineShader");
	m_FullScreenShader = CShaderManager::GetInst()->FindShader("FullScreenShader");
	m_LightCombineRenderShader = CShaderManager::GetInst()->FindShader("LightCombineRenderShader");

	m_ShadowMapShader = CShaderManager::GetInst()->FindShader("ShadowShader");

	// 그림자 맵용 깊이버퍼 생성
	// 깊이버퍼를 만들어준다.
	D3D11_TEXTURE2D_DESC    DepthDesc = {};

	DepthDesc.Width = m_ShadowMapSize;
	DepthDesc.Height = m_ShadowMapSize;
	DepthDesc.ArraySize = 1;

	DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthDesc.SampleDesc.Count = 1;
	DepthDesc.SampleDesc.Quality = 0;
	DepthDesc.MipLevels = 1;

	ID3D11Texture2D* pDepthStencilTex = nullptr;

	CDevice::GetInst()->GetDevice()->CreateTexture2D(&DepthDesc, nullptr, &pDepthStencilTex);

	CDevice::GetInst()->GetDevice()->CreateDepthStencilView(pDepthStencilTex, nullptr, &m_ShadowDepth);

	SAFE_RELEASE(pDepthStencilTex);
}

void CRenderManager::Render(float DeltaTime)
{
	m_LightCombineTarget->ClearTarget();

	size_t	Size = m_vecGBuffer.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecGBuffer[i]->ClearTarget();
	}

	Size = m_vecLight.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecLight[i]->ClearTarget();
	}

	if (m_RenderSpace == Render_Space::Render3D)
	Render3D(DeltaTime);

	else
	{
		Render2D(DeltaTime);
	}

	// UI를 출력한다.
	CScene* Scene = CSceneManager::GetInst()->GetScene();

	CViewport* Viewport = Scene->GetViewport();

	m_DepthDisable->SetState();
	m_UIAlphaBlend->SetState();

	Viewport->Render();

	if (m_MouseWidget)
		m_MouseWidget->Render();

	m_UIAlphaBlend->ResetState();

	// 디버깅용 렌더타겟 출력
	CResourceManager::GetInst()->RenderTarget();
	

	m_DepthDisable->ResetState();
}

void CRenderManager::Render2D(float DeltaTime)
{
	// 렌더타겟을 백버퍼에서 우리가 만들어둔 DiffuseTarget으로 교체가 된다.
	// 깊이버퍼는 nullptr을 넣어주었기 때문에 기존에 사용하던 깊이버퍼가
	// 그대로 들어가게 된다.
	m_DiffuseTarget->ClearTarget();
	m_DiffuseTarget->SetTarget(nullptr);

	m_PostProcessCBuffer->UpdateCBuffer();

	for (int i = 0; i < RST_End; ++i)
	{
		if (m_WorldState[i])
			m_WorldState[i]->SetState();
	}

	std::sort(m_pRenderList2D[RT2D_MAP], m_pRenderList2D[RT2D_MAP] + m_RenderList2DCount[RT2D_MAP],
		CRenderManager::Sort2DObject);

	m_DepthDisable->SetState();

	for (int j = 0; j < m_RenderList2DCount[RT2D_MAP]; ++j)
	{
		m_pRenderList2D[RT2D_MAP][j]->Render(DeltaTime);
	}

	m_DepthDisable->ResetState();

	for (int i = RT2D_Back; i < RT2D_End; ++i)
	{
		std::sort(m_pRenderList2D[i], m_pRenderList2D[i] + m_RenderList2DCount[i],
			CRenderManager::Sort2DObject);

		for (int j = 0; j < m_RenderList2DCount[i]; ++j)
		{
			m_pRenderList2D[i][j]->Render(DeltaTime);
		}
	}


	for (int i = 0; i < RT2D_End; ++i)
	{
		m_RenderList2DCount[i] = 0;
	}

	for (int i = 0; i < RST_End; ++i)
	{
		if (m_WorldState[i])
			m_WorldState[i]->ResetState();
	}

	// 출력이 끝났다면 원래의 타겟으로 되돌려준다.
	m_DiffuseTarget->ResetTarget();


	// 만들어진 Diffuse Target을 Distortion Target에 그려낸다.
	// 이때 깊이버퍼에 영향이 가지 않도록 깊이는 꺼준다.
	m_DistortionTarget->ClearTarget();
	m_DistortionTarget->SetTarget(nullptr);

	m_DepthDisable->SetState();
	m_AlphaBlend->SetState();

	m_DiffuseTargetMaterial->SetMaterial();

	CONTEXT->IASetInputLayout(nullptr);

	UINT	Offset = 0;
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	CONTEXT->Draw(4, 0);

	m_DiffuseTargetMaterial->ResetMaterial();


	m_AlphaBlend->ResetState();
	m_DepthDisable->ResetState();

	m_DiffuseTarget->SetShader(104, TST_PIXEL);

	// 실제 Distortion 물체들을 그려낸다.
	for (int i = 0; i < m_RenderListDistortionCount; ++i)
	{
		m_pRenderListDistortion[i]->Render(DeltaTime);
	}

	m_RenderListDistortionCount = 0;

	m_DiffuseTarget->ResetShader(104, TST_PIXEL);

	m_DistortionTarget->ResetTarget();

	// 만들어진 최종 화면을 백버퍼에 출력한다.
	m_DistortionTargetMaterial->SetMaterial();

	m_DistortionCBuffer->UpdateCBuffer();

	CONTEXT->IASetInputLayout(nullptr);

	Offset = 0;
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	CONTEXT->Draw(4, 0);

	m_DistortionTargetMaterial->ResetMaterial();

}

void CRenderManager::Render3D(float DeltaTime)
{
	// Sky 출력
	CPrimitiveComponent* SkyMesh = (CPrimitiveComponent*)CSceneManager::GetInst()->GetScene()->GetSkyMesh();

	m_SkyState->SetState();
	m_CullNone->SetState();

	SkyMesh->Render(DeltaTime);

	m_CullNone->ResetState();
	m_SkyState->ResetState();

	// 그림자 맵을 그려낸다.
	//RenderShadow(DeltaTime);

	// GBuffer를 그려낸다.
	RenderGBuffer(DeltaTime);

	// Decal을 그려낸다.
	RenderDecal(DeltaTime);

	RenderDecalDebug(DeltaTime);

	// 조명 누적버퍼를 그려낸다.
	RenderLightAcc(DeltaTime);

	// 조명 누적버퍼와 Albedo를 합성하여 최종 조명처리된 타겟을 만들어낸다.
	RenderLightCombine(DeltaTime);

	// 최종 조명 처리된 타겟을 백버퍼에 출력한다.
	RenderCombineTarget(DeltaTime);

	// 반투명 물체인 파티클은 따로 그려낸다.
	m_AlphaBlend->SetState();

	m_vecGBuffer[2]->SetShader(50, TST_PIXEL);

	for (int j = 0; j < m_RenderList3DCount[RT3D_Particle]; ++j)
	{
		m_pRenderList3D[RT3D_Particle][j]->Render(DeltaTime);
	}

	m_vecGBuffer[2]->ResetShader(50, TST_PIXEL);

	m_AlphaBlend->ResetState();

	for (int i = 0; i < RT3D_End; ++i)
	{
		m_RenderList3DCount[i] = 0;
	}
}

void CRenderManager::RenderGBuffer(float DeltaTime)
{
	// 렌더타겟을 GBuffer로 교체하여 그린다.
	std::vector<ID3D11RenderTargetView*>	vecTarget;
	std::vector<ID3D11RenderTargetView*>	vecPrevTarget;
	ID3D11DepthStencilView* PrevDepthTarget = nullptr;

	size_t	Size = m_vecGBuffer.size();

	vecPrevTarget.resize(Size);

	for (size_t i = 0; i < Size; ++i)
	{
		vecTarget.push_back(m_vecGBuffer[i]->GetTargetView());
	}

	CONTEXT->OMGetRenderTargets((unsigned int)Size, &vecPrevTarget[0], &PrevDepthTarget);

	ID3D11DepthStencilView* Depth = PrevDepthTarget;

	CONTEXT->OMSetRenderTargets((unsigned int)Size, &vecTarget[0], Depth);


	for (int j = 0; j < m_RenderList3DCount[RT3D_TERRAIN]; ++j)
	{
		m_pRenderList3D[RT3D_TERRAIN][j]->Render(DeltaTime);
	}

	for (int j = 0; j < m_RenderList3DCount[RT3D_Default]; ++j)
	{
		m_pRenderList3D[RT3D_Default][j]->Render(DeltaTime);
	}

	CONTEXT->OMSetRenderTargets(1, &vecPrevTarget[0], PrevDepthTarget);
	SAFE_RELEASE(PrevDepthTarget);

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_RELEASE(vecPrevTarget[0]);
	}
}

void CRenderManager::RenderShadow(float DeltaTime)
{
	D3D11_VIEWPORT	VP = {};
	VP.Width = m_ShadowMapSize;
	VP.Height = m_ShadowMapSize;
	VP.MaxDepth = 1.f;

	D3D11_VIEWPORT	PrevVP = {};

	unsigned int	ViewportCount = 1;
	CONTEXT->RSGetViewports(&ViewportCount, &PrevVP);
	CONTEXT->RSSetViewports(1, &VP);

	CONTEXT->ClearDepthStencilView(m_ShadowDepth, D3D11_CLEAR_DEPTH, 1.f, 0);

	m_ShadowTarget->ClearTarget();

	// 그림자맵용 DepthBuffer 지정
	m_ShadowTarget->SetTarget(m_ShadowDepth);

	// 그림자맵 출력용 Shader 지정
	m_ShadowMapShader->SetShader();


	// 그림자맵에 그려질 물체 출력
	for (int j = 0; j < m_RenderList3DCount[RT3D_TERRAIN]; ++j)
	{
		m_pRenderList3D[RT3D_TERRAIN][j]->RenderShadow(DeltaTime);
	}

	for (int j = 0; j < m_RenderList3DCount[RT3D_Default]; ++j)
	{
		m_pRenderList3D[RT3D_Default][j]->RenderShadow(DeltaTime);
	}

	m_ShadowTarget->ResetTarget();

	CONTEXT->RSSetViewports(1, &PrevVP);
}

void CRenderManager::RenderDecal(float DeltaTime)
{
	// 렌더타겟을 Decal Buffer로 교체하여 그린다.
	std::vector<ID3D11RenderTargetView*>	vecTarget;
	std::vector<ID3D11RenderTargetView*>	vecPrevTarget;
	ID3D11DepthStencilView* PrevDepthTarget = nullptr;

	size_t	Size = m_vecDecal.size();

	vecPrevTarget.resize(Size);

	for (size_t i = 0; i < Size; ++i)
	{
		vecTarget.push_back(m_vecDecal[i]->GetTargetView());
	}

	CONTEXT->OMGetRenderTargets((unsigned int)Size, &vecPrevTarget[0], &PrevDepthTarget);

	ID3D11DepthStencilView* Depth = PrevDepthTarget;

	CONTEXT->OMSetRenderTargets((unsigned int)Size, &vecTarget[0], Depth);

	m_vecGBuffer[2]->SetShader(10, TST_PIXEL);
	m_vecGBuffer[4]->SetShader(11, TST_PIXEL);
	m_vecGBuffer[5]->SetShader(12, TST_PIXEL);

	m_DepthWriteDisable->SetState();
	m_CullNone->SetState();
	m_DecalBlend->SetState();

	for (int j = 0; j < m_RenderList3DCount[RT3D_Decal]; ++j)
	{
		m_pRenderList3D[RT3D_Decal][j]->Render(DeltaTime);
	}

	m_DecalBlend->ResetState();
	m_CullNone->ResetState();
	m_DepthWriteDisable->ResetState();

	m_vecGBuffer[2]->ResetShader(10, TST_PIXEL);
	m_vecGBuffer[4]->ResetShader(11, TST_PIXEL);
	m_vecGBuffer[5]->ResetShader(12, TST_PIXEL);

	CONTEXT->OMSetRenderTargets(1, &vecPrevTarget[0], PrevDepthTarget);
	SAFE_RELEASE(PrevDepthTarget);

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_RELEASE(vecPrevTarget[0]);
	}
}

void CRenderManager::RenderDecalDebug(float DeltaTime)
{
	// 렌더타겟을 GBuffer로 교체하여 그린다.
	std::vector<ID3D11RenderTargetView*>	vecTarget;
	std::vector<ID3D11RenderTargetView*>	vecPrevTarget;
	ID3D11DepthStencilView* PrevDepthTarget = nullptr;

	size_t	Size = m_vecGBuffer.size();

	vecPrevTarget.resize(Size);

	for (size_t i = 0; i < Size; ++i)
	{
		vecTarget.push_back(m_vecGBuffer[i]->GetTargetView());
	}

	CONTEXT->OMGetRenderTargets((unsigned int)Size, &vecPrevTarget[0], &PrevDepthTarget);

	ID3D11DepthStencilView* Depth = PrevDepthTarget;

	CONTEXT->OMSetRenderTargets((unsigned int)Size, &vecTarget[0], Depth);


	m_DepthWriteDisable->SetState();
	m_CullNone->SetState();

	for (int j = 0; j < m_RenderList3DCount[RT3D_Decal]; ++j)
	{
		m_pRenderList3D[RT3D_Decal][j]->RenderDebug(DeltaTime);
	}

	m_CullNone->ResetState();
	m_DepthWriteDisable->ResetState();

	CONTEXT->OMSetRenderTargets(1, &vecPrevTarget[0], PrevDepthTarget);
	SAFE_RELEASE(PrevDepthTarget);

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_RELEASE(vecPrevTarget[0]);
	}
}

void CRenderManager::RenderLightAcc(float DeltaTime)
{
	// 렌더타겟을 조명 누적버퍼로 교체하여 그린다.
	std::vector<ID3D11RenderTargetView*>	vecTarget;
	std::vector<ID3D11RenderTargetView*>	vecPrevTarget;
	ID3D11DepthStencilView* PrevDepthTarget = nullptr;

	size_t	Size = m_vecLight.size();

	vecPrevTarget.resize(Size);

	for (size_t i = 0; i < Size; ++i)
	{
		vecTarget.push_back(m_vecLight[i]->GetTargetView());
	}

	CONTEXT->OMGetRenderTargets((unsigned int)Size, &vecPrevTarget[0], &PrevDepthTarget);

	ID3D11DepthStencilView* Depth = PrevDepthTarget;

	CONTEXT->OMSetRenderTargets((unsigned int)Size, &vecTarget[0], Depth);

	//m_DepthWriteDisable->SetState();

	// GBuffer를 Shader로 넘겨준다.
	size_t	GBufferSize = m_vecGBuffer.size();

	for (size_t i = 0; i < GBufferSize; ++i)
	{
		m_vecGBuffer[i]->SetShader(10 + (int)i, TST_PIXEL);
	}

	// 조명 종류에 따라 출력한다.
	CSceneManager::GetInst()->GetScene()->GetLightManager()->Render();

	for (size_t i = 0; i < GBufferSize; ++i)
	{
		m_vecGBuffer[i]->ResetShader(10 + (int)i, TST_PIXEL);
	}

	//m_DepthWriteDisable->ResetState();

	CONTEXT->OMSetRenderTargets(1, &vecPrevTarget[0], PrevDepthTarget);
	SAFE_RELEASE(PrevDepthTarget);

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_RELEASE(vecPrevTarget[0]);
	}
}

void CRenderManager::RenderLightCombine(float DeltaTime)
{
	m_LightCombineTarget->SetTarget(nullptr);


	m_DepthDisable->SetState();

	m_vecGBuffer[0]->SetShader(10, TST_PIXEL);
	m_vecGBuffer[2]->SetShader(12, TST_PIXEL);

	m_ShadowTarget->SetShader(17, TST_PIXEL);

	size_t	Size = m_vecLight.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecLight[i]->SetShader(14 + (int)i, TST_PIXEL);
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_ShadowBias -= DeltaTime * 0.01f;
	}


	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_ShadowBias += DeltaTime * 0.01f;
	}

	char	Test[256] = {};

	sprintf_s(Test, "Bias : %.5f\n", m_ShadowBias);
	OutputDebugStringA(Test);


	// 그림자를 그리기 위한 광원에서의 뷰 행렬과 투영 행렬을 만들어서 넘겨주면
	// Shader에서 월드공간의 위치를 얻어와서 이 행렬을 곱하여 광원에서의 정보를
	// 만들어내고 그림자 부분인지를 판단한다.
	Matrix	matShadow;

	CScene* Scene = CSceneManager::GetInst()->GetScene();
	CCamera* Camera = Scene->GetCameraManager()->GetCurrentCamera();

	matShadow = Camera->GetShadowViewMatrix() * Camera->GetShadowProjMatrix();
	matShadow.Transpose();

	Matrix	matShadowInvVP = Camera->GetViewMatrix() * Camera->GetProjMatrix();
	matShadowInvVP.Inverse();
	matShadowInvVP.Transpose();

	m_ShadowCBuffer->SetShadowMatrix(matShadow);
	m_ShadowCBuffer->SetShadowInvVPMatrix(matShadowInvVP);
	m_ShadowCBuffer->SetShadowBias(m_ShadowBias);

	m_ShadowCBuffer->UpdateCBuffer();



	m_LightCombineShader->SetShader();

	CONTEXT->IASetInputLayout(nullptr);

	UINT Offset = 0;
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	CONTEXT->Draw(4, 0);

	m_vecGBuffer[0]->ResetShader(10, TST_PIXEL);
	m_vecGBuffer[2]->ResetShader(12, TST_PIXEL);

	m_ShadowTarget->ResetShader(17, TST_PIXEL);

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecLight[i]->ResetShader(14 + (int)i, TST_PIXEL);
	}

	m_DepthDisable->ResetState();

	m_LightCombineTarget->ResetTarget();
}

void CRenderManager::RenderCombineTarget(float DeltaTime)
{
	m_DepthDisable->SetState();

	m_LightCombineTarget->SetShader(0, TST_PIXEL);

	m_LightCombineRenderShader->SetShader();

	//CONTEXT->IASetInputLayout(nullptr);

	UINT Offset = 0;
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	CONTEXT->Draw(4, 0);

	m_LightCombineTarget->ResetShader(0, TST_PIXEL);

	m_DepthDisable->ResetState();
}

void CRenderManager::AddPrimitiveComponent(CPrimitiveComponent* pPrimitive)
{
	if (pPrimitive->GetPrimitiveType() == PrimitiveComponent_Type::Primitive2D)
	{
		// 화면에 그려져야 하는지 판단한다.
		CScene* Scene = CSceneManager::GetInst()->GetScene();

		if (pPrimitive->GetRender2DType() != RT2D_MAP)
		{
			CCamera* Camera = Scene->GetCameraManager()->GetCurrentCamera();

			Resolution	RS = CDevice::GetInst()->GetResolution();

			Vector3	CameraLB = Camera->GetWorldPos();
			Vector3	CameraRT = CameraLB + Vector3((float)RS.Width, (float)RS.Height, 0.f);

			Vector3	PrimitiveLB = pPrimitive->GetWorldPos() - pPrimitive->GetPivot() * pPrimitive->GetWorldScale();
			Vector3	PrimitiveRT = PrimitiveLB + pPrimitive->GetWorldScale();

			if (CameraLB.x >= PrimitiveRT.x)
				return;

			else if (CameraRT.x <= PrimitiveLB.x)
				return;

			else if (CameraLB.y >= PrimitiveRT.y)
				return;

			else if (CameraRT.y <= PrimitiveLB.y)
				return;
		}

		if (pPrimitive->GetDistortionEnable())
		{
			if (m_RenderListDistortionCapacity == 0)
			{
				m_RenderListDistortionCapacity = 16;
				m_pRenderListDistortion = new CPrimitiveComponent * [m_RenderListDistortionCapacity];

				memset(m_pRenderListDistortion, 0, sizeof(CPrimitiveComponent*) * m_RenderListDistortionCapacity);
			}

			else if (m_RenderListDistortionCapacity == m_RenderListDistortionCount)
			{
				m_RenderListDistortionCapacity *= 2;

				CPrimitiveComponent** pArray = new CPrimitiveComponent * [m_RenderListDistortionCapacity];

				memset(pArray, 0, sizeof(CPrimitiveComponent*) * m_RenderListDistortionCapacity);

				memcpy(pArray, m_pRenderListDistortion, sizeof(CPrimitiveComponent*) * m_RenderListDistortionCount);

				SAFE_DELETE_ARRAY(m_pRenderListDistortion);

				m_pRenderListDistortion = pArray;
			}

			m_pRenderListDistortion[m_RenderListDistortionCount] = pPrimitive;
			++m_RenderListDistortionCount;
		}

		else
		{
			Render_Type_2D  Type2D = pPrimitive->GetRender2DType();

			if (m_RenderList2DCapacity[Type2D] == 0)
			{
				m_RenderList2DCapacity[Type2D] = 16;
				m_pRenderList2D[Type2D] = new CPrimitiveComponent * [m_RenderList2DCapacity[Type2D]];

				memset(m_pRenderList2D[Type2D], 0, sizeof(CPrimitiveComponent*) * m_RenderList2DCapacity[Type2D]);
			}

			else if (m_RenderList2DCapacity[Type2D] == m_RenderList2DCount[Type2D])
			{
				m_RenderList2DCapacity[Type2D] *= 2;

				CPrimitiveComponent** pArray = new CPrimitiveComponent * [m_RenderList2DCapacity[Type2D]];

				memset(pArray, 0, sizeof(CPrimitiveComponent*) * m_RenderList2DCapacity[Type2D]);

				memcpy(pArray, m_pRenderList2D[Type2D], sizeof(CPrimitiveComponent*) * m_RenderList2DCount[Type2D]);

				SAFE_DELETE_ARRAY(m_pRenderList2D[Type2D]);

				m_pRenderList2D[Type2D] = pArray;
			}

			m_pRenderList2D[Type2D][m_RenderList2DCount[Type2D]] = pPrimitive;
			++m_RenderList2DCount[Type2D];
		}
	}

	else if (pPrimitive->GetPrimitiveType() == PrimitiveComponent_Type::Primitive3D)
	{
		// 화면에 그려져야 하는지 판단한다.
		CScene* Scene = CSceneManager::GetInst()->GetScene();

		if (pPrimitive->GetRender3DType() != RT3D_TERRAIN)
		{
			CCamera* Camera = Scene->GetCameraManager()->GetCurrentCamera();

			/*Resolution	RS = CDevice::GetInst()->GetResolution();

			Vector3	CameraLB = Camera->GetWorldPos();
			Vector3	CameraRT = CameraLB + Vector3((float)RS.Width, (float)RS.Height, 0.f);

			Vector3	PrimitiveLB = pPrimitive->GetWorldPos() - pPrimitive->GetPivot() * pPrimitive->GetWorldScale();
			Vector3	PrimitiveRT = PrimitiveLB + pPrimitive->GetWorldScale();

			if (CameraLB.x >= PrimitiveRT.x)
				return;

			else if (CameraRT.x <= PrimitiveLB.x)
				return;

			else if (CameraLB.y >= PrimitiveRT.y)
				return;

			else if (CameraRT.y <= PrimitiveLB.y)
				return;*/
		}

		/*if (pPrimitive->GetDistortionEnable())
		{
			if (m_RenderListDistortionCapacity == 0)
			{
				m_RenderListDistortionCapacity = 16;
				m_pRenderListDistortion = new CPrimitiveComponent * [m_RenderListDistortionCapacity];

				memset(m_pRenderListDistortion, 0, sizeof(CPrimitiveComponent*) * m_RenderListDistortionCapacity);
			}

			else if (m_RenderListDistortionCapacity == m_RenderListDistortionCount)
			{
				m_RenderListDistortionCapacity *= 2;

				CPrimitiveComponent** pArray = new CPrimitiveComponent * [m_RenderListDistortionCapacity];

				memset(pArray, 0, sizeof(CPrimitiveComponent*) * m_RenderListDistortionCapacity);

				memcpy(pArray, m_pRenderListDistortion, sizeof(CPrimitiveComponent*) * m_RenderListDistortionCount);

				SAFE_DELETE_ARRAY(m_pRenderListDistortion);

				m_pRenderListDistortion = pArray;
			}

			m_pRenderListDistortion[m_RenderListDistortionCount] = pPrimitive;
			++m_RenderListDistortionCount;
		}*/

		//else
		{
			Render_Type_3D  Type3D = pPrimitive->GetRender3DType();

			if (m_RenderList3DCapacity[Type3D] == 0)
			{
				m_RenderList3DCapacity[Type3D] = 16;
				m_pRenderList3D[Type3D] = new CPrimitiveComponent * [m_RenderList3DCapacity[Type3D]];

				memset(m_pRenderList3D[Type3D], 0, sizeof(CPrimitiveComponent*) * m_RenderList3DCapacity[Type3D]);
			}

			else if (m_RenderList3DCapacity[Type3D] == m_RenderList3DCount[Type3D])
			{
				m_RenderList3DCapacity[Type3D] *= 2;

				CPrimitiveComponent** pArray = new CPrimitiveComponent * [m_RenderList3DCapacity[Type3D]];

				memset(pArray, 0, sizeof(CPrimitiveComponent*) * m_RenderList3DCapacity[Type3D]);

				memcpy(pArray, m_pRenderList3D[Type3D], sizeof(CPrimitiveComponent*) * m_RenderList3DCount[Type3D]);

				SAFE_DELETE_ARRAY(m_pRenderList3D[Type3D]);

				m_pRenderList3D[Type3D] = pArray;
			}

			m_pRenderList3D[Type3D][m_RenderList3DCount[Type3D]] = pPrimitive;
			++m_RenderList3DCount[Type3D];
		}
	}

	// Widget일 경우
	else
	{
		if (m_RenderSpace == Render_Space::Render2D)
		{
			// 화면에 그려져야 하는지 판단한다.
			CScene* Scene = CSceneManager::GetInst()->GetScene();

			CCamera* Camera = Scene->GetCameraManager()->GetCurrentCamera();

			Resolution	RS = CDevice::GetInst()->GetResolution();

			Vector3	CameraLB = Camera->GetWorldPos();
			Vector3	CameraRT = CameraLB + Vector3((float)RS.Width, (float)RS.Height, 0.f);

			Vector3	PrimitiveLB = pPrimitive->GetWorldPos() - pPrimitive->GetPivot() * pPrimitive->GetWorldScale();
			Vector3	PrimitiveRT = PrimitiveLB + pPrimitive->GetWorldScale();

			if (CameraLB.x >= PrimitiveRT.x)
				return;

			else if (CameraRT.x <= PrimitiveLB.x)
				return;

			else if (CameraLB.y >= PrimitiveRT.y)
				return;

			else if (CameraRT.y <= PrimitiveLB.y)
				return;

			if (pPrimitive->GetDistortionEnable())
			{
				if (m_RenderListDistortionCapacity == 0)
				{
					m_RenderListDistortionCapacity = 16;
					m_pRenderListDistortion = new CPrimitiveComponent * [m_RenderListDistortionCapacity];

					memset(m_pRenderListDistortion, 0, sizeof(CPrimitiveComponent*) * m_RenderListDistortionCapacity);
				}

				else if (m_RenderListDistortionCapacity == m_RenderListDistortionCount)
				{
					m_RenderListDistortionCapacity *= 2;

					CPrimitiveComponent** pArray = new CPrimitiveComponent * [m_RenderListDistortionCapacity];

					memset(pArray, 0, sizeof(CPrimitiveComponent*) * m_RenderListDistortionCapacity);

					memcpy(pArray, m_pRenderListDistortion, sizeof(CPrimitiveComponent*) * m_RenderListDistortionCount);

					SAFE_DELETE_ARRAY(m_pRenderListDistortion);

					m_pRenderListDistortion = pArray;
				}

				m_pRenderListDistortion[m_RenderListDistortionCount] = pPrimitive;
				++m_RenderListDistortionCount;
			}

			else
			{
				Render_Type_2D  Type2D = pPrimitive->GetRender2DType();

				if (m_RenderList2DCapacity[Type2D] == 0)
				{
					m_RenderList2DCapacity[Type2D] = 16;
					m_pRenderList2D[Type2D] = new CPrimitiveComponent * [m_RenderList2DCapacity[Type2D]];

					memset(m_pRenderList2D[Type2D], 0, sizeof(CPrimitiveComponent*) * m_RenderList2DCapacity[Type2D]);
				}

				else if (m_RenderList2DCapacity[Type2D] == m_RenderList2DCount[Type2D])
				{
					m_RenderList2DCapacity[Type2D] *= 2;

					CPrimitiveComponent** pArray = new CPrimitiveComponent * [m_RenderList2DCapacity[Type2D]];

					memset(pArray, 0, sizeof(CPrimitiveComponent*) * m_RenderList2DCapacity[Type2D]);

					memcpy(pArray, m_pRenderList2D[Type2D], sizeof(CPrimitiveComponent*) * m_RenderList2DCount[Type2D]);

					SAFE_DELETE_ARRAY(m_pRenderList2D[Type2D]);

					m_pRenderList2D[Type2D] = pArray;
				}

				m_pRenderList2D[Type2D][m_RenderList2DCount[Type2D]] = pPrimitive;
				++m_RenderList2DCount[Type2D];
			}
		}

		else
		{
		}
	}
}

bool CRenderManager::Sort2DObject(CPrimitiveComponent* Src,
	CPrimitiveComponent* Dest)
{
	float	SrcY = Src->GetWorldPos().y - Src->GetPivot().y * Src->GetWorldScale().y;
	float	DestY = Dest->GetWorldPos().y - Dest->GetPivot().y * Dest->GetWorldScale().y;

	return SrcY > DestY;
}

bool CRenderManager::Sort3DObject(CPrimitiveComponent* Src, CPrimitiveComponent* Dest)
{
	return false;
}

bool CRenderManager::Sort3DAlpha(CPrimitiveComponent* Src, CPrimitiveComponent* Dest)
{
	return false;
}
