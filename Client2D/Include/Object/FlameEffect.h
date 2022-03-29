#pragma once

#include "GameObject.h"
#include "Component/ParticleSystemComponent.h"
#include "Component/DistortionComponent.h"

class CFlameEffect :
    public CGameObject
{
	friend class CScene;

protected:
	CFlameEffect();
	CFlameEffect(const CFlameEffect& obj);
	virtual ~CFlameEffect();

protected:
	CSharedPtr<CParticleSystemComponent> m_Particle;
	CSharedPtr<CDistortionComponent>	m_Distortion;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CFlameEffect* Clone();
	virtual void Animation2DNotify(const std::string& Name);
};

