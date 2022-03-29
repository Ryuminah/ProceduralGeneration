#pragma once
#include "GraphicShader.h"
class CShadowShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CShadowShader();
    virtual ~CShadowShader();

public:
    virtual bool Init();
};

