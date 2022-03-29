#pragma once

#include "GraphicShader.h"

class CBillboardShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CBillboardShader();
    virtual ~CBillboardShader();

public:
    virtual bool Init();
};

