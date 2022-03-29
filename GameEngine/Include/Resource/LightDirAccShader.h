#pragma once

#include "GraphicShader.h"

class CLightDirAccShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CLightDirAccShader();
    virtual ~CLightDirAccShader();

public:
    virtual bool Init();
};

