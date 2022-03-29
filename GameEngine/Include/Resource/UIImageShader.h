#pragma once
#include "GraphicShader.h"
class CUIImageShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CUIImageShader();
    virtual ~CUIImageShader();

public:
    virtual bool Init();
};

