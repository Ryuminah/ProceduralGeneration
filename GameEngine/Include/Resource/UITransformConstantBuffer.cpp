
#include "UITransformConstantBuffer.h"
#include "ConstantBuffer.h"

CUITransformConstantBuffer::CUITransformConstantBuffer() :
    m_BufferData{}
{
}

CUITransformConstantBuffer::CUITransformConstantBuffer(const CUITransformConstantBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CUITransformConstantBuffer::~CUITransformConstantBuffer()
{
}

bool CUITransformConstantBuffer::Init()
{
    SetConstantBuffer("UITransformCBuffer");

    return true;
}

void CUITransformConstantBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CUITransformConstantBuffer* CUITransformConstantBuffer::Clone()
{
    return new CUITransformConstantBuffer(*this);
}
