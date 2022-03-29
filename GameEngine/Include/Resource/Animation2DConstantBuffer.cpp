
#include "Animation2DConstantBuffer.h"
#include "ConstantBuffer.h"

CAnimation2DConstantBuffer::CAnimation2DConstantBuffer()    :
    m_BufferData{}
{
}

CAnimation2DConstantBuffer::CAnimation2DConstantBuffer(const CAnimation2DConstantBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CAnimation2DConstantBuffer::~CAnimation2DConstantBuffer()
{
}

bool CAnimation2DConstantBuffer::Init()
{
    SetConstantBuffer("Animation2DCBuffer");

    return true;
}

void CAnimation2DConstantBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CAnimation2DConstantBuffer* CAnimation2DConstantBuffer::Clone()
{
    return new CAnimation2DConstantBuffer(*this);
}
