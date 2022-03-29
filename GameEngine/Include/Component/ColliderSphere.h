#pragma once
#include "Collider.h"
class CColliderSphere :
    public CCollider
{
	friend class CGameObject;

protected:
	CColliderSphere();
	CColliderSphere(const CColliderSphere& com);
	virtual ~CColliderSphere();

protected:
	SphereInfo	m_Info;

public:
	SphereInfo GetInfo()	const
	{
		return m_Info;
	}

public:
	void SetRadius(float Radius)
	{
		m_Info.Radius = Radius;

		SetWorldScale(m_Info.Radius, m_Info.Radius, m_Info.Radius);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CColliderSphere* Clone();

public:
	virtual bool Collision(CCollider* Dest);
	virtual bool CollisionMouse(const Vector2& MousePos);
};

