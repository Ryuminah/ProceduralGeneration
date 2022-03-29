
#include "UIColorTintCBuffer.h"
#include "ConstantBuffer.h"

CUIColorTintCBuffer::CUIColorTintCBuffer() :
    m_BufferData{}
{
    m_BufferData.Tint = Vector4(1.f, 1.f, 1.f, 1.f);
}

CUIColorTintCBuffer::CUIColorTintCBuffer(const CUIColorTintCBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CUIColorTintCBuffer::~CUIColorTintCBuffer()
{
}

bool CUIColorTintCBuffer::Init()
{
    SetConstantBuffer("UIColorTintCBuffer");

    return true;
}

void CUIColorTintCBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CUIColorTintCBuffer* CUIColorTintCBuffer::Clone()
{
    return new CUIColorTintCBuffer(*this);
}
