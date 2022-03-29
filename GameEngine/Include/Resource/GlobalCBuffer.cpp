
#include "GlobalCBuffer.h"
#include "ConstantBuffer.h"

CGlobalCBuffer::CGlobalCBuffer() :
    m_BufferData{}
{
}

CGlobalCBuffer::CGlobalCBuffer(const CGlobalCBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CGlobalCBuffer::~CGlobalCBuffer()
{
}

bool CGlobalCBuffer::Init()
{
    SetConstantBuffer("GlobalCBuffer");

    return true;
}

void CGlobalCBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CGlobalCBuffer* CGlobalCBuffer::Clone()
{
    return new CGlobalCBuffer(*this);
}
