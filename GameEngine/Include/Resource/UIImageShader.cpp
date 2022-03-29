
#include "UIImageShader.h"

CUIImageShader::CUIImageShader()
{
}

CUIImageShader::~CUIImageShader()
{
}

bool CUIImageShader::Init()
{
    if (!LoadVertexShader(m_Name, "UIMainVS", TEXT("UI.fx"), SHADER_PATH))
        return false;

    if (!LoadPixelShader(m_Name, "UIImagePS", TEXT("UI.fx"), SHADER_PATH))
        return false;

    AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
    AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

    if (!CreateInputLayout())
        return false;

    return true;
}
