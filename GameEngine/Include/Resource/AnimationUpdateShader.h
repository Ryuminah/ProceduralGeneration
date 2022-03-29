#pragma once
#include "ComputeShader.h"
class CAnimationUpdateShader :
    public CComputeShader
{
    friend class CShaderManager;

protected:
    CAnimationUpdateShader();
    virtual ~CAnimationUpdateShader();

public:
    virtual bool Init();
};

