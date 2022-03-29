
#include "NumberCBuffer.h"
#include "ConstantBuffer.h"

CNumberCBuffer::CNumberCBuffer() :
    m_BufferData{}
{
}

CNumberCBuffer::CNumberCBuffer(const CNumberCBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CNumberCBuffer::~CNumberCBuffer()
{
}

bool CNumberCBuffer::Init()
{
    SetConstantBuffer("NumberCBuffer");

    return true;
}

void CNumberCBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CNumberCBuffer* CNumberCBuffer::Clone()
{
    return new CNumberCBuffer(*this);
}
