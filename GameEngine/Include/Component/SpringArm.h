#pragma once
#include "SceneComponent.h"
class CSpringArm :
    public CSceneComponent
{
	friend class CGameObject;

protected:
	CSpringArm();
	CSpringArm(const CSpringArm& com);
	virtual ~CSpringArm();

protected:
	Vector3	m_Offset;
	float	m_TargetLength;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CSpringArm* Clone();
};

