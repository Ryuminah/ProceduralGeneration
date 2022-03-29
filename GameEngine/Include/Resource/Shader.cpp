
#include "Shader.h"

CShader::CShader()  :
    m_InputLayout(nullptr),
    m_InputSize(0)
{
}

CShader::~CShader()
{
    SAFE_RELEASE(m_InputLayout);
}

bool CShader::Init()
{
    return true;
}

void CShader::AddInputDesc(const char* SemanticName, unsigned int SemanticIndex, DXGI_FORMAT Fmt,
    unsigned int InputSlot, unsigned int Size, D3D11_INPUT_CLASSIFICATION InputSlotClass,
    unsigned int InstanceDataStepRate)
{
    D3D11_INPUT_ELEMENT_DESC    Desc = {};

    Desc.SemanticName = SemanticName;
    Desc.SemanticIndex = SemanticIndex;
    Desc.Format = Fmt;
    Desc.InputSlot = InputSlot;
    Desc.InputSlotClass = InputSlotClass;
    Desc.InstanceDataStepRate = InstanceDataStepRate;
    Desc.AlignedByteOffset = m_InputSize;

    m_InputSize += Size;

    m_vecInputDesc.push_back(Desc);
}

bool CShader::CreateInputLayout()
{
    return true;
}

void CShader::SetShader()
{
}
