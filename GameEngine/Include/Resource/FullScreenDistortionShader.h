#pragma once
#include "GraphicShader.h"
class CFullScreenDistortionShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CFullScreenDistortionShader();
    virtual ~CFullScreenDistortionShader();

public:
    virtual bool Init();
};

