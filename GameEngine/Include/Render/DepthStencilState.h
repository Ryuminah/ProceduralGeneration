#pragma once
#include "RenderState.h"
class CDepthStencilState :
    public CRenderState
{
	friend class CRenderStateManager;

protected:
	CDepthStencilState();
	virtual ~CDepthStencilState();

private:
	unsigned int	m_StencilRef;
	unsigned int	m_PrevStencilRef;

public:
	void SetStencilRef(unsigned int StencilRef)
	{
		m_StencilRef = StencilRef;
	}

public:
	bool CreateState(bool DepthEnable, 
		D3D11_DEPTH_WRITE_MASK DepthWriteMask,
		D3D11_COMPARISON_FUNC DepthFunc,
		bool StencilEnable,
		UINT8 StencilReadMask,
		UINT8 StencilWriteMask,
		D3D11_DEPTH_STENCILOP_DESC FrontFace,
		D3D11_DEPTH_STENCILOP_DESC BackFace);

public:
	virtual void SetState();
	virtual void ResetState();
};

