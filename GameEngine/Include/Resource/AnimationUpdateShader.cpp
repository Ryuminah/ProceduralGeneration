
#include "AnimationUpdateShader.h"

CAnimationUpdateShader::CAnimationUpdateShader()
{
}

CAnimationUpdateShader::~CAnimationUpdateShader()
{
}

bool CAnimationUpdateShader::Init()
{
    if (!LoadComputeShader(m_Name, "AnimationUpdateCS", TEXT("AnimationUpdate.fx"), SHADER_PATH))
        return false;

    return true;
}
