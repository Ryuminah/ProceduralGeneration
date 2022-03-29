#pragma once

#include "GraphicShader.h"

class CSkyShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CSkyShader();
    virtual ~CSkyShader();

public:
    virtual bool Init();
};

