
#include "RasterizerState.h"
#include "../Device.h"

CRasterizerState::CRasterizerState()
{
	m_Type = RST_Rasterizer;
}

CRasterizerState::~CRasterizerState()
{
}

bool CRasterizerState::CreateState(D3D11_FILL_MODE FillMode, 
	D3D11_CULL_MODE CullMode, bool FrontCounterClockwise, INT DepthBias, 
	FLOAT DepthBiasClamp, FLOAT SlopeScaledDepthBias, bool DepthClipEnable,
	bool ScissorEnable, bool MultisampleEnable, bool AntialiasedLineEnable)
{
	D3D11_RASTERIZER_DESC	Desc = {};

	Desc.FillMode = FillMode;
	Desc.CullMode = CullMode;
	Desc.FrontCounterClockwise = FrontCounterClockwise;
	Desc.DepthBias = DepthBias;
	Desc.DepthBiasClamp = DepthBiasClamp;
	Desc.SlopeScaledDepthBias = SlopeScaledDepthBias;
	Desc.DepthClipEnable = DepthClipEnable;
	Desc.ScissorEnable = ScissorEnable;
	Desc.MultisampleEnable = MultisampleEnable;
	Desc.AntialiasedLineEnable = AntialiasedLineEnable;

	if (FAILED(DEVICE->CreateRasterizerState(&Desc, (ID3D11RasterizerState**)&m_State)))
		return false;

	return true;
}

void CRasterizerState::SetState()
{
	CONTEXT->RSGetState((ID3D11RasterizerState**)&m_PrevState);
	CONTEXT->RSSetState((ID3D11RasterizerState*)m_State);
}

void CRasterizerState::ResetState()
{
	CONTEXT->RSSetState((ID3D11RasterizerState*)m_PrevState);
	SAFE_RELEASE(m_PrevState);
}
