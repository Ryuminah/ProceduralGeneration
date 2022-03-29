
#include "BlendState.h"
#include "../Device.h"

CBlendState::CBlendState()
{
	m_SampleMask = 0xffffffff;
	m_PrevSampleMask = 0;
	memset(m_BlendFactor, 0, sizeof(float) * 4);
	memset(m_PrevBlendFactor, 0, sizeof(float) * 4);

	m_Type = RST_Blend;
}

CBlendState::~CBlendState()
{
}

void CBlendState::AddBlendInfo(bool BlendEnable, D3D11_BLEND SrcBlend, 
	D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp,
	D3D11_BLEND SrcBlendAlpha, D3D11_BLEND DestBlendAlpha, 
	D3D11_BLEND_OP BlendOpAlpha, UINT8 RenderTargetWriteMask)
{
	D3D11_RENDER_TARGET_BLEND_DESC	Desc = {};

	Desc.BlendEnable = BlendEnable;
	Desc.SrcBlend = SrcBlend;
	Desc.DestBlend = DestBlend;
	Desc.BlendOp = BlendOp;
	Desc.SrcBlendAlpha = SrcBlendAlpha;
	Desc.DestBlendAlpha = DestBlendAlpha;
	Desc.BlendOpAlpha = BlendOpAlpha;
	Desc.RenderTargetWriteMask = RenderTargetWriteMask;

	m_vecDesc.push_back(Desc);
}

bool CBlendState::CreateBlendState(bool AlphaToCoverageEnable, 
	bool IndependentBlendEnable)
{
	D3D11_BLEND_DESC	Desc = {};

	Desc.AlphaToCoverageEnable = AlphaToCoverageEnable;
	Desc.IndependentBlendEnable = IndependentBlendEnable;

	memcpy(Desc.RenderTarget, &m_vecDesc[0],
		sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * m_vecDesc.size());

	if (FAILED(DEVICE->CreateBlendState(&Desc, (ID3D11BlendState**)&m_State)))
		return false;

	return true;
}

void CBlendState::SetState()
{
	CONTEXT->OMGetBlendState((ID3D11BlendState**)&m_PrevState,
		m_PrevBlendFactor, &m_PrevSampleMask);

	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_State, m_BlendFactor,
		m_SampleMask);
}

void CBlendState::ResetState()
{
	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_PrevState,
		m_PrevBlendFactor, m_PrevSampleMask);
	SAFE_RELEASE(m_PrevState);
}
