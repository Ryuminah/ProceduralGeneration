
#include "RenderStateManager.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"

DEFINITION_SINGLE(CRenderStateManager)

CRenderStateManager::CRenderStateManager()
{
}

CRenderStateManager::~CRenderStateManager()
{
	auto	iter = m_mapRenderState.begin();
	auto	iterEnd = m_mapRenderState.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CRenderStateManager::Init()
{
	AddBlendInfo("AlphaBlend");

	if (!CreateBlendState("AlphaBlend", true))
		return false;

	AddBlendInfo("UIAlphaBlend");

	if (!CreateBlendState("UIAlphaBlend", false))
		return false;

	if (!CreateDepthStencilState("DepthDisable", false))
		return false;

	if (!CreateRasterizerState("WireFrame", D3D11_FILL_WIREFRAME))
		return false;

	return true;
}

bool CRenderStateManager::AddBlendInfo(const std::string& Name, 
	bool BlendEnable, D3D11_BLEND SrcBlend, D3D11_BLEND DestBlend,
	D3D11_BLEND_OP BlendOp, D3D11_BLEND SrcBlendAlpha, 
	D3D11_BLEND DestBlendAlpha, D3D11_BLEND_OP BlendOpAlpha, 
	UINT8 RenderTargetWriteMask)
{
	CBlendState* State = (CBlendState*)FindRenderState(Name);

	if (!State)
	{
		State = new CBlendState;

		State->SetName(Name);

		m_mapRenderState.insert(std::make_pair(Name, State));
	}

	State->AddBlendInfo(BlendEnable, SrcBlend, DestBlend, BlendOp,
		SrcBlendAlpha, DestBlendAlpha, BlendOpAlpha, RenderTargetWriteMask);

	return true;
}

bool CRenderStateManager::CreateBlendState(const std::string& Name, 
	bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	CBlendState* State = (CBlendState*)FindRenderState(Name);

	if (!State)
		return false;

	if (!State->CreateBlendState(AlphaToCoverageEnable, IndependentBlendEnable))
	{
		SAFE_DELETE(State);
		return false;
	}

	return true;
}

bool CRenderStateManager::CreateDepthStencilState(const std::string& Name, bool DepthEnable,
	D3D11_DEPTH_WRITE_MASK DepthWriteMask, D3D11_COMPARISON_FUNC DepthFunc, 
	bool StencilEnable, UINT8 StencilReadMask, UINT8 StencilWriteMask, 
	D3D11_DEPTH_STENCILOP_DESC FrontFace, D3D11_DEPTH_STENCILOP_DESC BackFace)
{
	CDepthStencilState* State = (CDepthStencilState*)FindRenderState(Name);

	if (State)
		return false;

	State = new CDepthStencilState;

	State->SetName(Name);

	if (!State->CreateState(DepthEnable, DepthWriteMask,
		DepthFunc, StencilEnable, StencilReadMask,
		StencilWriteMask, FrontFace, BackFace))
	{
		SAFE_DELETE(State);
		return false;
	}

	m_mapRenderState.insert(std::make_pair(Name, State));

	return true;
}

bool CRenderStateManager::CreateRasterizerState(const std::string& Name, 
	D3D11_FILL_MODE FillMode, D3D11_CULL_MODE CullMode, 
	bool FrontCounterClockwise, INT DepthBias, FLOAT DepthBiasClamp,
	FLOAT SlopeScaledDepthBias, bool DepthClipEnable, bool ScissorEnable,
	bool MultisampleEnable, bool AntialiasedLineEnable)
{
	CRasterizerState* State = (CRasterizerState*)FindRenderState(Name);

	if (State)
		return false;

	State = new CRasterizerState;

	State->SetName(Name);

	if (!State->CreateState(FillMode, CullMode,
		FrontCounterClockwise, DepthBias, DepthBiasClamp,
		SlopeScaledDepthBias, DepthClipEnable, ScissorEnable,
		MultisampleEnable, AntialiasedLineEnable))
	{
		SAFE_DELETE(State);
		return false;
	}

	m_mapRenderState.insert(std::make_pair(Name, State));

	return true;
}

CRenderState* CRenderStateManager::FindRenderState(const std::string& Name) const
{
	auto	iter = m_mapRenderState.find(Name);

	if (iter == m_mapRenderState.end())
		return nullptr;

	return iter->second;
}
