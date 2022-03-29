#pragma once

#include "GraphicShader.h"

class CCollider3DShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CCollider3DShader();
    virtual ~CCollider3DShader();

public:
    virtual bool Init();
};

