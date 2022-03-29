#pragma once
#include "GraphicShader.h"
class CTileMapShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CTileMapShader();
    virtual ~CTileMapShader();

public:
    virtual bool Init();
};

