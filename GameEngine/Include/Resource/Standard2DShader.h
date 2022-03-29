#pragma once

#include "GraphicShader.h"

class CStandard2DShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CStandard2DShader();
    virtual ~CStandard2DShader();

public:
    virtual bool Init();
};

