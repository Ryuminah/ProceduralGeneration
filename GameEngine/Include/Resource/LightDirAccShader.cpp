
#include "LightDirAccShader.h"

CLightDirAccShader::CLightDirAccShader()
{
}

CLightDirAccShader::~CLightDirAccShader()
{
}

bool CLightDirAccShader::Init()
{
    if (!LoadVertexShader(m_Name, "LightDirVS", TEXT("Light.fx"), SHADER_PATH))
        return false;

    if (!LoadPixelShader(m_Name, "LightAccPS", TEXT("Light.fx"), SHADER_PATH))
        return false;

    return true;
}
