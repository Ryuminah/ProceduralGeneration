
#include "LandScapeCBuffer.h"
#include "ConstantBuffer.h"

CLandScapeCBuffer::CLandScapeCBuffer() :
    m_BufferData{}
{
    m_BufferData.DetailLevel = 1.f;
}

CLandScapeCBuffer::CLandScapeCBuffer(const CLandScapeCBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CLandScapeCBuffer::~CLandScapeCBuffer()
{
}

bool CLandScapeCBuffer::Init()
{
    SetConstantBuffer("LandScapeCBuffer");

    return true;
}

void CLandScapeCBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CLandScapeCBuffer* CLandScapeCBuffer::Clone()
{
    return new CLandScapeCBuffer(*this);
}
