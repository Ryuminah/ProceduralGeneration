#pragma once

#include "Monster.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

class CTeemo :
    public CMonster
{
	friend class CScene;

protected:
	CTeemo();
	CTeemo(const CTeemo& obj);
	virtual ~CTeemo();

protected:
	CSharedPtr<CSceneComponent> m_Muzzle;

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
	virtual CTeemo* Clone();
	virtual void Animation2DNotify(const std::string& Name);

public:
	void AnimationFrameEnd(const std::string& Name);
};

