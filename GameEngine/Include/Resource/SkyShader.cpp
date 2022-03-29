
#include "SkyShader.h"

CSkyShader::CSkyShader()
{
}

CSkyShader::~CSkyShader()
{
}

bool CSkyShader::Init()
{
    if (!LoadVertexShader(m_Name, "SkyVS", TEXT("Sky.fx"), SHADER_PATH))
        return false;

    if (!LoadPixelShader(m_Name, "SkyPS", TEXT("Sky.fx"), SHADER_PATH))
        return false;

    AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

    if (!CreateInputLayout())
        return false;

    return true;
}
