#pragma once
#include "GraphicShader.h"
class CRenderTargetShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CRenderTargetShader();
    virtual ~CRenderTargetShader();

public:
    virtual bool Init();
};

