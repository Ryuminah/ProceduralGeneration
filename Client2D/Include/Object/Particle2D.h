#pragma once

#include "GameObject.h"
#include "Component/ParticleSystemComponent.h"
class CParticle2D :
    public CGameObject
{
	friend class CScene;

protected:
	CParticle2D();
	CParticle2D(const CParticle2D& obj);
	virtual ~CParticle2D();

protected:
	CSharedPtr<CParticleSystemComponent> m_Particle;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CParticle2D* Clone();
	virtual void Animation2DNotify(const std::string& Name);
};

