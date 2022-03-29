#pragma once
#include "SceneComponent.h"
class CSpringArm3D :
    public CSceneComponent
{
	friend class CGameObject;

protected:
	CSpringArm3D();
	CSpringArm3D(const CSpringArm3D& com);
	virtual ~CSpringArm3D();

protected:
	Vector3	m_Offset;
	float	m_Length;

public:
	void SetOffset(float x, float y, float z)
	{
		m_Offset = Vector3(x, y, z);
	}

	void SetOffset(const Vector3& Offset)
	{
		m_Offset = Offset;
	}

	void SetLength(float Length)
	{
		m_Length = Length;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CSpringArm3D* Clone();
};

