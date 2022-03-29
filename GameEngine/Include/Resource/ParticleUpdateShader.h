#pragma once
#include "ComputeShader.h"
class CParticleUpdateShader :
    public CComputeShader
{
    friend class CShaderManager;

protected:
    CParticleUpdateShader();
    virtual ~CParticleUpdateShader();

public:
    virtual bool Init();
};

