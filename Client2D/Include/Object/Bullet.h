#pragma once

#include "GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"

class CBullet :
    public CGameObject
{
    friend class CScene;

protected:
    CBullet();
    CBullet(const CBullet& obj);
    virtual ~CBullet();

protected:
    CSharedPtr<CSpriteComponent> m_Sprite;
    CSharedPtr<CColliderCircle> m_Body;
    float       m_Distance;
    bool        m_Collision;

public:
    void SetDistance(float Distance)
    {
        m_Distance = Distance;
    }

    void SetCollisionProfile(const std::string& Name)
    {
        m_Body->SetCollisionProfile(Name);
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(float DeltaTime);
    virtual CBullet* Clone();

public:
    void CollisionBegin(const HitResult& result, CCollider* Collider);
    void CollisionEnd(const HitResult& result, CCollider* Collider);
};

