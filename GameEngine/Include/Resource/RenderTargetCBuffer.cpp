
#include "RenderTargetCBuffer.h"
#include "ConstantBuffer.h"

CRenderTargetCBuffer::CRenderTargetCBuffer() :
    m_BufferData{}
{
}

CRenderTargetCBuffer::CRenderTargetCBuffer(const CRenderTargetCBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CRenderTargetCBuffer::~CRenderTargetCBuffer()
{
}

bool CRenderTargetCBuffer::Init()
{
    SetConstantBuffer("RenderTargetCBuffer");

    return true;
}

void CRenderTargetCBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CRenderTargetCBuffer* CRenderTargetCBuffer::Clone()
{
    return new CRenderTargetCBuffer(*this);
}
