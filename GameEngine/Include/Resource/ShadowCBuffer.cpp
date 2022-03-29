
#include "ShadowCBuffer.h"
#include "ConstantBuffer.h"

CShadowCBuffer::CShadowCBuffer() :
    m_BufferData{}
{
}

CShadowCBuffer::CShadowCBuffer(const CShadowCBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CShadowCBuffer::~CShadowCBuffer()
{
}

bool CShadowCBuffer::Init()
{
    SetConstantBuffer("ShadowCBuffer");

    return true;
}

void CShadowCBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CShadowCBuffer* CShadowCBuffer::Clone()
{
    return new CShadowCBuffer(*this);
}
