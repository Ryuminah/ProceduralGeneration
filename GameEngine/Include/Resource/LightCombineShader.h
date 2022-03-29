#pragma once
#include "GraphicShader.h"
class CLightCombineShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CLightCombineShader();
    virtual ~CLightCombineShader();

public:
    virtual bool Init();
};

