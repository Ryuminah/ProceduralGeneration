#pragma once

#include "GraphicShader.h"

class CLandScapeShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CLandScapeShader();
    virtual ~CLandScapeShader();

public:
    virtual bool Init();
};

