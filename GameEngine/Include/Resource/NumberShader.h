#pragma once
#include "GraphicShader.h"
class CNumberShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CNumberShader();
    virtual ~CNumberShader();

public:
    virtual bool Init();
};

