
#include "ProgressBarCBuffer.h"
#include "ConstantBuffer.h"

CProgressBarCBuffer::CProgressBarCBuffer() :
    m_BufferData{}
{
    m_BufferData.Percent = 1.f;
}

CProgressBarCBuffer::CProgressBarCBuffer(const CProgressBarCBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CProgressBarCBuffer::~CProgressBarCBuffer()
{
}

bool CProgressBarCBuffer::Init()
{
    SetConstantBuffer("ProgressBarCBuffer");

    return true;
}

void CProgressBarCBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CProgressBarCBuffer* CProgressBarCBuffer::Clone()
{
    return new CProgressBarCBuffer(*this);
}
