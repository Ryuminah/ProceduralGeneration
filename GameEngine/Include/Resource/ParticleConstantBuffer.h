#pragma once

#include "ConstantBufferData.h"

class CParticleConstantBuffer :
	public CConstantBufferData
{
public:
	CParticleConstantBuffer();
	CParticleConstantBuffer(const CParticleConstantBuffer& buffer);
	virtual ~CParticleConstantBuffer();

protected:
	ParticleCBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CParticleConstantBuffer* Clone();

public:
	const ParticleCBuffer& GetBuffer()	const
	{
		return m_BufferData;
	}

public:
	void SetGravityEnable(int Enable)
	{
		m_BufferData.Gravity = Enable;
	}

	void SetDefaultZ(float Z)
	{
		m_BufferData.DefaultZ = Z;
	}

	void SetStartColor(const Vector4& Color)
	{
		m_BufferData.StartColor = Color;
	}

	void SetStartColor(float r, float g, float b, float a)
	{
		m_BufferData.StartColor = Vector4(r, g, b, a);
	}

	void SetEndColor(const Vector4& Color)
	{
		m_BufferData.EndColor = Color;
	}

	void SetEndColor(float r, float g, float b, float a)
	{
		m_BufferData.EndColor = Vector4(r, g, b, a);
	}

	void SetMaxParticleCount(unsigned int MaxCount)
	{
		m_BufferData.SpawnCountMax = MaxCount;
	}

	void SetStartScale(const Vector3& Scale)
	{
		m_BufferData.StartScale = Scale;
	}

	void SetStartScale(float x, float y, float z)
	{
		m_BufferData.StartScale = Vector3(x, y, z);
	}

	void SetEndScale(const Vector3& Scale)
	{
		m_BufferData.EndScale = Scale;
	}

	void SetEndScale(float x, float y, float z)
	{
		m_BufferData.EndScale = Vector3(x, y, z);
	}

	void SetPos(const Vector3& Pos)
	{
		m_BufferData.Pos = Pos;
	}

	void SetPos(float x, float y, float z)
	{
		m_BufferData.Pos = Vector3(x, y, z);
	}

	void SetLifeTimeMin(float Min)
	{
		m_BufferData.LifeTimeMin = Min;
	}

	void SetLifeTimeMax(float Max)
	{
		m_BufferData.LifeTimeMax = Max;
	}

	void SetRange(const Vector3& Range)
	{
		m_BufferData.Range = Range;
	}

	void SetRange(float x, float y, float z)
	{
		m_BufferData.Range = Vector3(x, y, z);
	}

	void SetMinSpeed(float Min)
	{
		m_BufferData.MinSpeed = Min;
	}

	void SetMaxSpeed(float Max)
	{
		m_BufferData.MaxSpeed = Max;
	}

	void SetMoveEnable(bool Move)
	{
		m_BufferData.Move = Move ? 1 : 0;
	}

	void SetParticle2D(bool b2D)
	{
		m_BufferData.Particle2D = b2D ? 1 : 0;
	}

	void SetMoveDir(const Vector3& Dir)
	{
		m_BufferData.MoveDir = Dir;
	}

	void SetMoveDir(float x, float y, float z)
	{
		m_BufferData.MoveDir = Vector3(x, y, z);
	}

	void SetMoveAngle(const Vector3& Angle)
	{
		m_BufferData.MoveAngle = Angle;
	}

	void SetMoveAngle(float x, float y, float z)
	{
		m_BufferData.MoveAngle = Vector3(x, y, z);
	}

	void SetSpawnCount(unsigned int Count)
	{
		m_BufferData.SpawnCount = Count;
	}

	int GetSpawnCountMax()	const
	{
		return m_BufferData.SpawnCountMax;
	}
};

