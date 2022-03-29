#pragma once
#include "Collider.h"
class CColliderCircle :
    public CCollider
{
	friend class CGameObject;

protected:
	CColliderCircle();
	CColliderCircle(const CColliderCircle& com);
	virtual ~CColliderCircle();

protected:
	CircleInfo	m_Info;

public:
	CircleInfo GetInfo()	const
	{
		return m_Info;
	}

public:
	void SetRadius(float Radius)
	{
		m_Info.Radius = Radius;

		SetWorldScale(m_Info.Radius, m_Info.Radius, 1.f);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CColliderCircle* Clone();

public:
	virtual bool Collision(CCollider* Dest);
	virtual bool CollisionMouse(const Vector2& MousePos);
};

