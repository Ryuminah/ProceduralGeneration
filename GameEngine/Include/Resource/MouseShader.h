#pragma once
#include "GraphicShader.h"
class CMouseShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CMouseShader();
    virtual ~CMouseShader();

public:
    virtual bool Init();
};

