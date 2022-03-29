#pragma once

#include "GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderPixel.h"

class CPixelCollisionTest :
    public CGameObject
{
	friend class CScene;

protected:
	CPixelCollisionTest();
	CPixelCollisionTest(const CPixelCollisionTest& obj);
	virtual ~CPixelCollisionTest();

protected:
	CSharedPtr<CSpriteComponent> m_Sprite;
	CSharedPtr<CColliderPixel> m_Body;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CPixelCollisionTest* Clone();
};

