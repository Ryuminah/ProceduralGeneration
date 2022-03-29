
#include "FullScreenShader.h"

CFullScreenShader::CFullScreenShader()
{
}

CFullScreenShader::~CFullScreenShader()
{
}

bool CFullScreenShader::Init()
{
    if (!LoadVertexShader(m_Name, "FullScreenVS", TEXT("Share.fx"), SHADER_PATH))
        return false;

    if (!LoadPixelShader(m_Name, "FullScreenPS", TEXT("Share.fx"), SHADER_PATH))
        return false;

    return true;
}
