#pragma once

#include "GameObject.h"
#include "Component/ParticleSystemComponent.h"

class CTestParticle :
    public CGameObject
{
	friend class CScene;

protected:
	CTestParticle();
	CTestParticle(const CTestParticle& obj);
	virtual ~CTestParticle();

protected:
	CSharedPtr<CParticleSystemComponent> m_Particle;

protected:
	bool    m_ChildFire;
	float   m_ChildFireTime;
	float   m_ChildFireTimeMax;
	float   m_ChildFireDurationTime;
	float   m_ChildFireDurationTimeMax;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CTestParticle* Clone();
	virtual void Animation2DNotify(const std::string& Name);
};

