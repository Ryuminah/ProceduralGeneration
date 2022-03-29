#pragma once
#include "GraphicShader.h"
class CColliderShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CColliderShader();
    virtual ~CColliderShader();

public:
    virtual bool Init();
};

