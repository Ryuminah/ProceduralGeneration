
#include "Standard2DShader.h"

CStandard2DShader::CStandard2DShader()
{
}

CStandard2DShader::~CStandard2DShader()
{
}

bool CStandard2DShader::Init()
{
    if (!LoadVertexShader(m_Name, "Standard2DColorVS", TEXT("Standard2D.fx"), SHADER_PATH))
        return false;

    if (!LoadPixelShader(m_Name, "Standard2DColorPS", TEXT("Standard2D.fx"), SHADER_PATH))
        return false;

    AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
    AddInputDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

    if (!CreateInputLayout())
        return false;

    return true;
}
