
#include "AnimationUpdateConstantBuffer.h"
#include "ConstantBuffer.h"

CAnimationUpdateConstantBuffer::CAnimationUpdateConstantBuffer() :
    m_BufferData{}
{
}

CAnimationUpdateConstantBuffer::CAnimationUpdateConstantBuffer(const CAnimationUpdateConstantBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CAnimationUpdateConstantBuffer::~CAnimationUpdateConstantBuffer()
{
}

bool CAnimationUpdateConstantBuffer::Init()
{
    SetConstantBuffer("AnimationCBuffer");

    return true;
}

void CAnimationUpdateConstantBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CAnimationUpdateConstantBuffer* CAnimationUpdateConstantBuffer::Clone()
{
    return new CAnimationUpdateConstantBuffer(*this);
}
