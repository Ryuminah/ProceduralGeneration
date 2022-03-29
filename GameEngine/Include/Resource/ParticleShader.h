#pragma once
#include "GraphicShader.h"
class CParticleShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CParticleShader();
    virtual ~CParticleShader();

public:
    virtual bool Init();
};

