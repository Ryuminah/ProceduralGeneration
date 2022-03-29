
#include "ConstantBufferData.h"
#include "ShaderManager.h"

CConstantBufferData::CConstantBufferData()
{
}

CConstantBufferData::CConstantBufferData(const CConstantBufferData& buffer)
{
    m_pBuffer = buffer.m_pBuffer;
}

CConstantBufferData::~CConstantBufferData()
{
}

void CConstantBufferData::SetConstantBuffer(const std::string& Name)
{
    m_pBuffer = CShaderManager::GetInst()->FindConstantBuffer(Name);
}
