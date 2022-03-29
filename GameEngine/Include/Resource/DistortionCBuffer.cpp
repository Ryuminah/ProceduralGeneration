
#include "DistortionCBuffer.h"
#include "ConstantBuffer.h"

CDistortionCBuffer::CDistortionCBuffer() :
    m_BufferData{}
{
    m_BufferData.Weight = 0.01f;
    m_BufferData.Self = 0;
    m_BufferData.Speed = 0.1f;
}

CDistortionCBuffer::CDistortionCBuffer(const CDistortionCBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CDistortionCBuffer::~CDistortionCBuffer()
{
}

bool CDistortionCBuffer::Init()
{
    SetConstantBuffer("Distortion2DCBuffer");

    return true;
}

void CDistortionCBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CDistortionCBuffer* CDistortionCBuffer::Clone()
{
    return new CDistortionCBuffer(*this);
}
