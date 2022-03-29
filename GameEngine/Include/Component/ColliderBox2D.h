#pragma once
#include "Collider.h"
class CColliderBox2D :
    public CCollider
{
	friend class CGameObject;

protected:
	CColliderBox2D();
	CColliderBox2D(const CColliderBox2D& com);
	virtual ~CColliderBox2D();

protected:
	Box2DInfo	m_Info;

public:
	Box2DInfo GetInfo()	const
	{
		return m_Info;
	}

public:
	void SetExtent(float Width, float Height)
	{
		m_Info.Length[0] = Width;
		m_Info.Length[1] = Height;

		SetWorldScale(m_Info.Length[0] * 2.f, m_Info.Length[1] * 2.f, 1.f);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CColliderBox2D* Clone();

public:
	virtual bool Collision(CCollider* Dest);
	virtual bool CollisionMouse(const Vector2& MousePos);
};

