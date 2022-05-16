
#include "TransformConstantBuffer.h"
#include "ConstantBuffer.h"

CTransformConstantBuffer::CTransformConstantBuffer() :
    m_BufferData{}
{
}

CTransformConstantBuffer::CTransformConstantBuffer(const CTransformConstantBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CTransformConstantBuffer::~CTransformConstantBuffer()
{
}

bool CTransformConstantBuffer::Init()
{
    SetConstantBuffer("TransformCBuffer");

    return true;
}

void CTransformConstantBuffer::UpdateCBuffer()
{
    m_BufferData.matWV = m_BufferData.matWorld * m_BufferData.matView;
    m_BufferData.matVP = m_BufferData.matView * m_BufferData.matProj;
    m_BufferData.matWVP = m_BufferData.matWV * m_BufferData.matProj;

    // Shader에서는 행렬을 사용할때 C++에서 사용하는 행과 열의 반대로 사용이 된다.
    // 그래서 C++에서 사용하는 행렬 정보를 Shader에 넘겨줄때에는 반드시 Transpose를 해서
    // 넘겨주어야 한다.
    m_BufferData.matWorld.Transpose();
    m_BufferData.matView.Transpose();
    m_BufferData.matProj.Transpose();
    m_BufferData.matWV.Transpose();
    m_BufferData.matVP.Transpose();
    m_BufferData.matWVP.Transpose();

    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CTransformConstantBuffer* CTransformConstantBuffer::Clone()
{
    return new CTransformConstantBuffer(*this);
}
