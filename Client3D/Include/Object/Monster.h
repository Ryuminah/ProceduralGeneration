#pragma once

#include "GameObject.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderSphere.h"
#include "Component/ColliderCube.h"
#include "Resource/Animation.h"

class CMonster :
    public CGameObject
{
    friend class CScene;

protected:
    CMonster();
    CMonster(const CMonster& obj);
    virtual ~CMonster();

protected:
    CSharedPtr<CSceneComponent> m_Root;
    CSharedPtr<CAnimationMeshComponent> m_Mesh;
    CSharedPtr<CColliderCube> m_Body;
    CAnimation* m_Animation;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(float DeltaTime);
    virtual CMonster* Clone();
};

