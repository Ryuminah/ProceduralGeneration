
#include "LightConstantBuffer.h"
#include "ConstantBuffer.h"

CLightConstantBuffer::CLightConstantBuffer() :
    m_BufferData{}
{
    m_BufferData.Diffuse = Vector4::White;
    m_BufferData.Ambient = Vector4(0.2f, 0.2f, 0.2f, 1.f);
    m_BufferData.Specular = Vector4::White;
    m_BufferData.LightType = (int)Light_Type::Dir;
    m_BufferData.Dir = Vector3(0.f, 0.f, 1.f);
    m_BufferData.Distance = 5.f;
    m_BufferData.AngleIn = 15.f;
    m_BufferData.AngleOut = 25.f;
    m_BufferData.Att1 = 1.f;
    m_BufferData.Att2 = 0.09f;
    m_BufferData.Att3 = 0.03f;
}

CLightConstantBuffer::CLightConstantBuffer(const CLightConstantBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CLightConstantBuffer::~CLightConstantBuffer()
{
}

bool CLightConstantBuffer::Init()
{
    SetConstantBuffer("LightCBuffer");

    return true;
}

void CLightConstantBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CLightConstantBuffer* CLightConstantBuffer::Clone()
{
    return new CLightConstantBuffer(*this);
}
