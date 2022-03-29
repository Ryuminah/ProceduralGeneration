
#include "ParticleSystem.h"
#include "ParticleConstantBuffer.h"
#include "StructuredBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "ShaderManager.h"

CParticleSystem::CParticleSystem()	:
	m_CBuffer(nullptr),
	m_ParticleInfo{},
	m_ParticleInfoShare{},
	m_SpawnTime(0.f),
	m_2D(true),
	m_Scene(nullptr)
{
}

CParticleSystem::CParticleSystem(const CParticleSystem& particle)
{
	*this = particle;
}

CParticleSystem::~CParticleSystem()
{
	SAFE_DELETE(m_CBuffer);
	auto	iter = m_vecBuffer.begin();
	auto	iterEnd = m_vecBuffer.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}
}

bool CParticleSystem::Init()
{
	m_UpdateShader = (CParticleUpdateShader*)CShaderManager::GetInst()->FindShader("ParticleUpdateShader");

	// 상수버퍼를 생성한다.
	m_CBuffer = new CParticleConstantBuffer;

	m_CBuffer->Init();

	m_SpawnTime = 0.001f;

	AddStructuredBuffer("ParticleInfo", sizeof(ParticleInfo),
		m_CBuffer->GetBuffer().SpawnCountMax, 0);
	AddStructuredBuffer("ParticleInfoShared", sizeof(ParticleInfoShared),
		1, 1);

	return true;
}

void CParticleSystem::AddStructuredBuffer(const std::string& Name,
	unsigned int Size, unsigned int Count, int Register, bool Dynamic, 
	int ConstantBufferType)
{
	CStructuredBuffer* Buffer = new CStructuredBuffer;

	if (!Buffer->Init(Name, Size, Count, Register, Dynamic, ConstantBufferType))
	{
		SAFE_DELETE(Buffer);
		return;
	}

	m_vecBuffer.push_back(Buffer);
}

void CParticleSystem::SetMaxParticleCount(unsigned int Count)
{
	m_CBuffer->SetMaxParticleCount(Count);

	ResizeBuffer("ParticleInfo", sizeof(ParticleInfo),
		Count, 0);
}

void CParticleSystem::SetStartColor(const Vector4& Color)
{
	m_CBuffer->SetStartColor(Color);
}

void CParticleSystem::SetStartColor(float r, float g, float b, float a)
{
	m_CBuffer->SetStartColor(r, g, b, a);
}

void CParticleSystem::SetEndColor(const Vector4& Color)
{
	m_CBuffer->SetEndColor(Color);
}

void CParticleSystem::SetEndColor(float r, float g, float b, float a)
{
	m_CBuffer->SetEndColor(r, g, b, a);
}

void CParticleSystem::SetStartScale(const Vector3& Scale)
{
	m_CBuffer->SetStartScale(Scale);
}

void CParticleSystem::SetStartScale(float x, float y, float z)
{
	m_CBuffer->SetStartScale(x, y, z);
}

void CParticleSystem::SetEndScale(const Vector3& Scale)
{
	m_CBuffer->SetEndScale(Scale);
}

void CParticleSystem::SetEndScale(float x, float y, float z)
{
	m_CBuffer->SetEndScale(x, y, z);
}

void CParticleSystem::SetLifeTimeMin(float Min)
{
	m_CBuffer->SetLifeTimeMin(Min);
}

void CParticleSystem::SetLifeTimeMax(float Max)
{
	m_CBuffer->SetLifeTimeMax(Max);
}

void CParticleSystem::SetRange(const Vector3& Range)
{
	m_CBuffer->SetRange(Range);
}

void CParticleSystem::SetRange(float x, float y, float z)
{
	m_CBuffer->SetRange(x, y, z);
}

void CParticleSystem::SetMinSpeed(float Min)
{
	m_CBuffer->SetMinSpeed(Min);
}

void CParticleSystem::SetMaxSpeed(float Max)
{
	m_CBuffer->SetMaxSpeed(Max);
}

void CParticleSystem::SetMoveEnable(bool Move)
{
	m_CBuffer->SetMoveEnable(Move);
}

void CParticleSystem::SetParticle2D(bool b2D)
{
	m_CBuffer->SetParticle2D(b2D);
}

void CParticleSystem::SetMoveDir(const Vector3& Dir)
{
	m_CBuffer->SetMoveDir(Dir);
}

void CParticleSystem::SetMoveDir(float x, float y, float z)
{
	m_CBuffer->SetMoveDir(x, y, z);
}

void CParticleSystem::SetMoveAngle(const Vector3& Angle)
{
	m_CBuffer->SetMoveAngle(Angle);
}

void CParticleSystem::SetMoveAngle(float x, float y, float z)
{
	m_CBuffer->SetMoveAngle(x, y, z);
}

void CParticleSystem::SetGravityEnable(bool Enable)
{
	int	Gravity = Enable ? 1 : 0;

	m_CBuffer->SetGravityEnable(Gravity);
}

bool CParticleSystem::ResizeBuffer(const std::string& Name,
	unsigned int Size, unsigned int Count, unsigned int Register,
	bool Dynamic, int ConstantBufferType)
{
	size_t	BufferSize = m_vecBuffer.size();

	for (size_t i = 0; i < BufferSize; ++i)
	{
		if (m_vecBuffer[i]->GetName() == Name)
		{
			m_vecBuffer[i]->Init(Name, Size, Count, Register, Dynamic,
				ConstantBufferType);
			return true;
		}
	}

	return false;
}

CParticleConstantBuffer* CParticleSystem::CloneConstantBuffer() const
{
	return m_CBuffer->Clone();
}

CMaterial* CParticleSystem::CloneMaterial() const
{
	return m_Material->Clone();
}

CParticleUpdateShader* CParticleSystem::GetUpdateShader() const
{
	return m_UpdateShader;
}

void CParticleSystem::CloneStructuredBuffer(
	std::vector<class CStructuredBuffer*>& vecBuffer)
{
	size_t	Size = m_vecBuffer.size();

	for(size_t i = 0; i < Size; ++i)
	{
		CStructuredBuffer* Buffer = m_vecBuffer[i]->Clone();

		vecBuffer.push_back(Buffer);
	}
}

