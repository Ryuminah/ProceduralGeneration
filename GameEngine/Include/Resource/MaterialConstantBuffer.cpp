
#include "MaterialConstantBuffer.h"
#include "ConstantBuffer.h"

CMaterialConstantBuffer::CMaterialConstantBuffer()  :
    m_BufferData{}
{
}

CMaterialConstantBuffer::CMaterialConstantBuffer(const CMaterialConstantBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CMaterialConstantBuffer::~CMaterialConstantBuffer()
{
}

bool CMaterialConstantBuffer::Init()
{
    SetConstantBuffer("MaterialCBuffer");

    return true;
}

void CMaterialConstantBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CMaterialConstantBuffer* CMaterialConstantBuffer::Clone()
{
    return new CMaterialConstantBuffer(*this);
}
