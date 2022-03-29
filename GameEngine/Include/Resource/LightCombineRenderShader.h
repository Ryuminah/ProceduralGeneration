#pragma once
#include "GraphicShader.h"
class CLightCombineRenderShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CLightCombineRenderShader();
    virtual ~CLightCombineRenderShader();

public:
    virtual bool Init();
};

