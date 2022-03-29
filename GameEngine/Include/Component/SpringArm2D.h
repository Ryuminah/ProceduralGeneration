#pragma once
#include "SceneComponent.h"
class CSpringArm2D :
    public CSceneComponent
{
	friend class CGameObject;

protected:
	CSpringArm2D();
	CSpringArm2D(const CSpringArm2D& com);
	virtual ~CSpringArm2D();

protected:
	Vector3	m_Offset;

public:
	void SetOffset(float x, float y, float z)
	{
		m_Offset = Vector3(x, y, z);
	}

	void SetOffset(const Vector3& Offset)
	{
		m_Offset = Offset;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CSpringArm2D* Clone();
};

