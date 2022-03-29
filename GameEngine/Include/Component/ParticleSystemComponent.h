#pragma once
#include "PrimitiveComponent.h"
#include "../Resource/ParticleSystem.h"
#include "../Resource/Mesh.h"
#include "../Resource/Material.h"
#include "../Resource/ParticleUpdateShader.h"
#include "../Resource/ParticleConstantBuffer.h"


class CParticleSystemComponent :
	public CPrimitiveComponent
{
	friend class CGameObject;

protected:
	CParticleSystemComponent();
	CParticleSystemComponent(const CParticleSystemComponent& com);
	virtual ~CParticleSystemComponent();

protected:
	CSharedPtr<CParticleSystem> m_Particle;
	std::vector<class CStructuredBuffer*>   m_vecBuffer;
	CSharedPtr<CMesh>                       m_Mesh;
	CSharedPtr<CParticleUpdateShader>		m_UpdateShader;
	CSharedPtr<CShader>                     m_RenderShader;
	CParticleConstantBuffer*		        m_CBuffer;
	ParticleInfo							m_ParticleInfo;
	ParticleInfoShared						m_ParticleInfoShare;
	float									m_SpawnTime;
	float									m_SpawnTimeMax;
	//class CStructuredBuffer* m_CopyBuffer;
	//ParticleInfo	m_Info[1000];

public:
	void SetParticle(const std::string& Name);
	void SetParticle(CParticleSystem* Particle);
	void SetSpawnTime(float Time);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CParticleSystemComponent* Clone();

public:
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
};

