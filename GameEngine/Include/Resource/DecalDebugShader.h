#pragma once
#include "GraphicShader.h"
class CDecalDebugShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CDecalDebugShader();
    virtual ~CDecalDebugShader();

public:
    virtual bool Init();
};

