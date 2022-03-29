#pragma once

#include "GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Component/PaperBurnComponent.h"

class CMonster :
    public CGameObject
{
	friend class CScene;

protected:
	CMonster();
	CMonster(const CMonster& obj);
	virtual ~CMonster();

protected:
	CSharedPtr<CSpriteComponent> m_Sprite;
	CSharedPtr<CColliderCircle> m_Body;
	CSharedPtr<CPaperBurnComponent> m_PaperBurn;
	class CAnimation2D* m_Animation2D;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CMonster* Clone();
	virtual void Animation2DNotify(const std::string& Name);

public:
	void AnimationFrameEnd(const std::string& Name);

public:
	void CollisionBegin(const HitResult& result, CCollider* Collider);
	void PaperBurnEndFunction();
};

