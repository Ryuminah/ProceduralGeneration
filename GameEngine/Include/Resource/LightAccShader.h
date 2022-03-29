#pragma once
#include "GraphicShader.h"
class CLightAccShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CLightAccShader();
    virtual ~CLightAccShader();

public:
    virtual bool Init();
};

