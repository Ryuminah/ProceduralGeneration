#pragma once
#include "Component.h"
class CObjectComponent :
    public CComponent
{
    friend class CGameObject;

protected:
    CObjectComponent();
    CObjectComponent(const CObjectComponent& com);
    virtual ~CObjectComponent();

protected:
    ObjectComponent_Type m_ObjectComponentType;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void PrevRender(float DeltaTime);
    virtual void Render(float DeltaTime);
    virtual CObjectComponent* Clone();
};

