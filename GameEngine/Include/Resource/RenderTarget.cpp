
#include "RenderTarget.h"
#include "../Device.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "../Component/Camera.h"

CRenderTarget::CRenderTarget()	:
	m_TargetView(nullptr),
	m_TargetTex(nullptr),
	m_PrevTargetView(nullptr),
	m_PrevDepthView(nullptr),
	m_Surface(nullptr),
	m_ClearColor{},
	m_DebugRender(false),
	m_Scale(100.f, 100.f, 1.f)
{
	m_IsTarget = true;
}

CRenderTarget::~CRenderTarget()
{
	SAFE_RELEASE(m_Surface);
	SAFE_RELEASE(m_PrevDepthView);
	SAFE_RELEASE(m_TargetView);
	SAFE_RELEASE(m_PrevTargetView);
	SAFE_RELEASE(m_TargetTex);
}

bool CRenderTarget::CreateTarget(const std::string& Name, 
	unsigned int Width, unsigned int Height, DXGI_FORMAT PixelFormat)
{
	// Target Texture 생성
	D3D11_TEXTURE2D_DESC	Desc = {};

	Desc.Width = Width;
	Desc.Height = Height;
	Desc.ArraySize = 1;
	Desc.MipLevels = 1;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Desc.Format = PixelFormat;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	if (FAILED(DEVICE->CreateTexture2D(&Desc, nullptr, &m_TargetTex)))
		return false;

	TextureResourceInfo* Info = new TextureResourceInfo;

	Info->Width = Width;
	Info->Height = Height;	

	m_vecResourceInfo.push_back(Info);

	m_TargetTex->QueryInterface(__uuidof(IDXGISurface), (void**)&m_Surface);

	// TargetTexture와 연결된 TargetView와 ShaderResourceView를 만들어준다.
	if (FAILED(DEVICE->CreateShaderResourceView(m_TargetTex, nullptr, &Info->SRV)))
		return false;

	if (FAILED(DEVICE->CreateRenderTargetView(m_TargetTex, nullptr, &m_TargetView)))
		return false;

	return true;
}

void CRenderTarget::ClearTarget()
{
	CONTEXT->ClearRenderTargetView(m_TargetView, m_ClearColor);
}

void CRenderTarget::SetTarget(ID3D11DepthStencilView* DepthView)
{
	CONTEXT->OMGetRenderTargets(1, &m_PrevTargetView, &m_PrevDepthView);

	ID3D11DepthStencilView* Depth = m_PrevDepthView;

	if (DepthView)
		Depth = DepthView;

	CONTEXT->OMSetRenderTargets(1, &m_TargetView, Depth);
}

void CRenderTarget::ResetTarget()
{
	CONTEXT->OMSetRenderTargets(1, &m_PrevTargetView, m_PrevDepthView);
	SAFE_RELEASE(m_PrevDepthView);
	SAFE_RELEASE(m_PrevTargetView);
}

void CRenderTarget::SetTargetShader()
{
	CCamera* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetUICamera();

	Matrix	matScale, matTrans;

	matScale.Scaling(m_Scale);
	matTrans.Translation(m_Pos);

	m_matDebugWVP = matScale * matTrans * Camera->GetProjMatrix();

	m_matDebugWVP.Transpose();

	CONTEXT->PSSetShaderResources(0, 1, &m_vecResourceInfo[0]->SRV);
}

void CRenderTarget::ResetTargetShader()
{
	ID3D11ShaderResourceView* SRV = nullptr;
	CONTEXT->PSSetShaderResources(0, 1, &SRV);
}
