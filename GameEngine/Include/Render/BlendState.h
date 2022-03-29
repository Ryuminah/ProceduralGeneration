#pragma once
#include "RenderState.h"
class CBlendState :
    public CRenderState
{
	friend class CRenderStateManager;

protected:
	CBlendState();
	virtual ~CBlendState();

public:
	std::vector<D3D11_RENDER_TARGET_BLEND_DESC>	m_vecDesc;
	unsigned int	m_SampleMask;
	unsigned int	m_PrevSampleMask;
	float			m_BlendFactor[4];
	float			m_PrevBlendFactor[4];

public:
	void AddBlendInfo(bool BlendEnable = true, D3D11_BLEND SrcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND DestBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP BlendOp = D3D11_BLEND_OP_ADD, 
		D3D11_BLEND SrcBlendAlpha = D3D11_BLEND_ONE, 
		D3D11_BLEND DestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP BlendOpAlpha = D3D11_BLEND_OP_ADD,
		UINT8 RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateBlendState(bool AlphaToCoverageEnable,
		bool IndependentBlendEnable);

public:
	virtual void SetState();
	virtual void ResetState();
};

