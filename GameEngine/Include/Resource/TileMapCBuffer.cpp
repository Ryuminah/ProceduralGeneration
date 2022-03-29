
#include "TileMapCBuffer.h"
#include "ConstantBuffer.h"

CTileMapCBuffer::CTileMapCBuffer() :
    m_BufferData{}
{
}

CTileMapCBuffer::CTileMapCBuffer(const CTileMapCBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CTileMapCBuffer::~CTileMapCBuffer()
{
}

bool CTileMapCBuffer::Init()
{
    SetConstantBuffer("TileMapCBuffer");

    return true;
}

void CTileMapCBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CTileMapCBuffer* CTileMapCBuffer::Clone()
{
    return new CTileMapCBuffer(*this);
}
