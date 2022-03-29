
#include "ConstantBuffer.h"
#include "../Device.h"

CConstantBuffer::CConstantBuffer()  :
    m_pBuffer(nullptr)
{
}

CConstantBuffer::~CConstantBuffer()
{
    SAFE_RELEASE(m_pBuffer);
}

bool CConstantBuffer::Init(const std::string& Name, int Size, int Register, int ConstantBufferType)
{
    m_Name = Name;
    m_Size = Size;
    m_Register = Register;
    m_ConstantBufferType = ConstantBufferType;

    D3D11_BUFFER_DESC   Desc = {};

    Desc.Usage = D3D11_USAGE_DYNAMIC;
    Desc.ByteWidth = Size;
    Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if (FAILED(DEVICE->CreateBuffer(&Desc, nullptr, &m_pBuffer)))
        return false;

    return true;
}

void CConstantBuffer::UpdateBuffer(void* pData)
{
    D3D11_MAPPED_SUBRESOURCE    Map = {};

    CONTEXT->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Map);

    // Map.pData 에 이 버퍼의 주소를 가지고 온것이다.
    // 여기에 데이터를 복사해준다.
    memcpy(Map.pData, pData, m_Size);

    CONTEXT->Unmap(m_pBuffer, 0);

    if (m_ConstantBufferType & CBT_VERTEX)
        CONTEXT->VSSetConstantBuffers(m_Register, 1, &m_pBuffer);

    if (m_ConstantBufferType & CBT_PIXEL)
        CONTEXT->PSSetConstantBuffers(m_Register, 1, &m_pBuffer);

    if (m_ConstantBufferType & CBT_DOMAIN)
        CONTEXT->DSSetConstantBuffers(m_Register, 1, &m_pBuffer);

    if (m_ConstantBufferType & CBT_HULL)
        CONTEXT->HSSetConstantBuffers(m_Register, 1, &m_pBuffer);

    if (m_ConstantBufferType & CBT_GEOMETRY)
        CONTEXT->GSSetConstantBuffers(m_Register, 1, &m_pBuffer);

    if (m_ConstantBufferType & CBT_COMPUTE)
        CONTEXT->CSSetConstantBuffers(m_Register, 1, &m_pBuffer);
}
