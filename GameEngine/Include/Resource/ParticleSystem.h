#pragma once

#include "../Ref.h"
#include "Material.h"
#include "ParticleUpdateShader.h"

class CParticleSystem :
	public CRef
{
	friend class CResourceManager;
	friend class CSceneResource;

private:
	CParticleSystem();
	CParticleSystem(const CParticleSystem& particle);
	~CParticleSystem();

private:
	class CScene*							m_Scene;
	std::vector<class CStructuredBuffer*>   m_vecBuffer;
	CSharedPtr<CMaterial>					m_Material;
	CSharedPtr<CParticleUpdateShader>		m_UpdateShader;
	ParticleInfo							m_ParticleInfo;
	ParticleInfoShared						m_ParticleInfoShare;
	class CParticleConstantBuffer*			m_CBuffer;
	float									m_SpawnTime;
	bool									m_2D;

public:
	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

public:
	bool Init();

protected:
	void AddStructuredBuffer(const std::string& Name, unsigned int Size, unsigned int Count,
		int Register, bool Dynamic = false,
		int ConstantBufferType = CBT_COMPUTE);

public:
	void SetMaterial(CMaterial* Material)
	{
		m_Material = Material;
	}
	void SetMaxParticleCount(unsigned int Count);

	void SetStartColor(const Vector4& Color);
	void SetStartColor(float r, float g, float b, float a);
	void SetEndColor(const Vector4& Color);
	void SetEndColor(float r, float g, float b, float a);

	void SetStartScale(const Vector3& Scale);
	void SetStartScale(float x, float y, float z);
	void SetEndScale(const Vector3& Scale);
	void SetEndScale(float x, float y, float z);

	void SetLifeTimeMin(float Min);
	void SetLifeTimeMax(float Max);
	void SetRange(const Vector3& Range);
	void SetRange(float x, float y, float z);
	void SetMinSpeed(float Min);
	void SetMaxSpeed(float Max);
	void SetMoveEnable(bool Move);
	void SetParticle2D(bool b2D);
	void SetMoveDir(const Vector3& Dir);
	void SetMoveDir(float x, float y, float z);
	void SetMoveAngle(const Vector3& Angle);
	void SetMoveAngle(float x, float y, float z);
	void SetGravityEnable(bool Enable);

protected:
	bool ResizeBuffer(const std::string& Name, unsigned int Size,
		unsigned int Count, unsigned int Register, bool Dynamic = false,
		int ConstantBufferType = CBT_COMPUTE);

public:
	class CParticleConstantBuffer* CloneConstantBuffer()	const;
	CMaterial* CloneMaterial()	const;
	CParticleUpdateShader* GetUpdateShader()	const;
	void CloneStructuredBuffer(std::vector<class CStructuredBuffer*>& vecBuffer);
	float GetSpawnTime()	const
	{
		return m_SpawnTime;
	}

	void SetSpawnTime(float Time)
	{
		m_SpawnTime = Time;
	}
};

