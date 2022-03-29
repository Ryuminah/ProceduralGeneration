#pragma once

#include "../Ref.h"

class CComponent :
    public CRef
{
    friend class CGameObject;

protected:
    CComponent();
    CComponent(const CComponent& com);
    virtual ~CComponent() = 0;

protected:
    class CScene* m_pScene;
    class CGameObject* m_pOwner;

public:
    class CGameObject* GetOwner()   const
    {
        return m_pOwner;
    }

    virtual void Active(bool bActive);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void PrevRender(float DeltaTime);
    virtual void Render(float DeltaTime);
    virtual void RenderDebug(float DeltaTime);
    virtual CComponent* Clone() = 0;
};

