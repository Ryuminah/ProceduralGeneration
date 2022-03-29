#pragma once
#include "GraphicShader.h"
class CStandard3DShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CStandard3DShader();
    virtual ~CStandard3DShader();

public:
    virtual bool Init();
};

