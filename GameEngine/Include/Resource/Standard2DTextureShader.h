#pragma once

#include "GraphicShader.h"

class CStandard2DTextureShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CStandard2DTextureShader();
    virtual ~CStandard2DTextureShader();

public:
    virtual bool Init();
};

