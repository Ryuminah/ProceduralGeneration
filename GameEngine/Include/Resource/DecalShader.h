#pragma once
#include "GraphicShader.h"
class CDecalShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CDecalShader();
    virtual ~CDecalShader();

public:
    virtual bool Init();
};

