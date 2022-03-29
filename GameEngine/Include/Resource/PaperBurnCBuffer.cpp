
#include "PaperBurnCBuffer.h"
#include "ConstantBuffer.h"

CPaperBurnCBuffer::CPaperBurnCBuffer() :
    m_BufferData{}
{
    m_BufferData.InLineColor = Vector3(0.8314f, 0.3922f, 0.f) * 2.f;
    m_BufferData.OutLineColor = Vector3(0.f, 0.f, 0.f);
    m_BufferData.CenterColor = Vector3(0.6667f, 0.3137f, 0.f);
    m_BufferData.OutFilter = 0.1f;
    m_BufferData.CenterFilter = 0.07f;
    m_BufferData.InFilter = 0.05f;
}

CPaperBurnCBuffer::CPaperBurnCBuffer(const CPaperBurnCBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CPaperBurnCBuffer::~CPaperBurnCBuffer()
{
}

bool CPaperBurnCBuffer::Init()
{
    SetConstantBuffer("PaperBurnCBuffer");

    return true;
}

void CPaperBurnCBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CPaperBurnCBuffer* CPaperBurnCBuffer::Clone()
{
    return new CPaperBurnCBuffer(*this);
}
