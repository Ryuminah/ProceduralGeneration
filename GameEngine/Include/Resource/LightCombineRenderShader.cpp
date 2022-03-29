
#include "LightCombineRenderShader.h"

CLightCombineRenderShader::CLightCombineRenderShader()
{
}

CLightCombineRenderShader::~CLightCombineRenderShader()
{
}

bool CLightCombineRenderShader::Init()
{
    if (!LoadVertexShader(m_Name, "LightCombineRenderVS", TEXT("Light.fx"), SHADER_PATH))
        return false;

    if (!LoadPixelShader(m_Name, "LightCombineRenderPS", TEXT("Light.fx"), SHADER_PATH))
        return false;

    return true;
}
