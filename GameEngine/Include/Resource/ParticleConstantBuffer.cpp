
#include "ParticleConstantBuffer.h"
#include "ConstantBuffer.h"

CParticleConstantBuffer::CParticleConstantBuffer() :
    m_BufferData{}
{
    m_BufferData.StartColor = Vector4(1.f, 1.f, 1.f, 1.f);
    m_BufferData.EndColor = Vector4(1.f, 1.f, 1.f, 1.f);
    m_BufferData.LifeTimeMin = 0.5f;
    m_BufferData.LifeTimeMax = 1.f;
    m_BufferData.StartScale = Vector3(30.f, 30.f, 1.f);
    m_BufferData.EndScale = Vector3(30.f, 30.f, 1.f);
    m_BufferData.MaxSpeed = 3.f;
    m_BufferData.MinSpeed = 1.f;
    m_BufferData.SpawnCountMax = 100;
    m_BufferData.Range = Vector3(3.f, 3.f, 1.f);
}

CParticleConstantBuffer::CParticleConstantBuffer(const CParticleConstantBuffer& buffer) :
    CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CParticleConstantBuffer::~CParticleConstantBuffer()
{
}

bool CParticleConstantBuffer::Init()
{
    SetConstantBuffer("ParticleCBuffer");

    return true;
}

void CParticleConstantBuffer::UpdateCBuffer()
{
    m_pBuffer->UpdateBuffer(&m_BufferData);
}

CParticleConstantBuffer* CParticleConstantBuffer::Clone()
{
    return new CParticleConstantBuffer(*this);
}
