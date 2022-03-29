
#include "FullScreenDistortionShader.h"

CFullScreenDistortionShader::CFullScreenDistortionShader()
{
}

CFullScreenDistortionShader::~CFullScreenDistortionShader()
{
}

bool CFullScreenDistortionShader::Init()
{
    if (!LoadVertexShader(m_Name, "FullScreenVS", TEXT("Share.fx"), SHADER_PATH))
        return false;

    if (!LoadPixelShader(m_Name, "FullScreenDistortionPS", TEXT("Share.fx"), SHADER_PATH))
        return false;

    return true;
}
