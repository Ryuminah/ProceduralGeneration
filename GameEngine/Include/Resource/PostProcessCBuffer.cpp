
#include "PostProcessCBuffer.h"
#include "ConstantBuffer.h"

CPostProcessCBuffer::CPostProcessCBuffer() :
    m_BufferData{}
{
}

CPostProcessCBuffer::CPostProcessCBuffer(const CPostProcessCBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CPostProcessCBuffer::~CPostProcessCBuffer()
{
}

bool CPostProcessCBuffer::Init()
{
    SetConstantBuffer("PostProcessCBuffer");

    return true;
}

void CPostProcessCBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CPostProcessCBuffer* CPostProcessCBuffer::Clone()
{
    return new CPostProcessCBuffer(*this);
}
