#pragma once
#include "RenderState.h"
class CRasterizerState :
    public CRenderState
{
	friend class CRenderStateManager;

protected:
	CRasterizerState();
	virtual ~CRasterizerState();

private:

public:
	bool CreateState(D3D11_FILL_MODE FillMode,
		D3D11_CULL_MODE CullMode,
		bool FrontCounterClockwise,
		INT DepthBias,
		FLOAT DepthBiasClamp,
		FLOAT SlopeScaledDepthBias,
		bool DepthClipEnable,
		bool ScissorEnable,
		bool MultisampleEnable,
		bool AntialiasedLineEnable);

public:
	virtual void SetState();
	virtual void ResetState();
};

