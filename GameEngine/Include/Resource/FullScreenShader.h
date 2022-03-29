#pragma once
#include "GraphicShader.h"
class CFullScreenShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CFullScreenShader();
    virtual ~CFullScreenShader();

public:
    virtual bool Init();
};

