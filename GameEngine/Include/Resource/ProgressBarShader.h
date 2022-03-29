#pragma once

#include "GraphicShader.h"

class CProgressBarShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CProgressBarShader();
    virtual ~CProgressBarShader();

public:
    virtual bool Init();
};

