#pragma once

#include "PrimitiveComponent.h"
#include "../Resource/StaticMesh.h"

class CBillboardComponent :
    public CPrimitiveComponent
{
    friend class CGameObject;

protected:
    CBillboardComponent();
    CBillboardComponent(const CBillboardComponent& com);
    virtual ~CBillboardComponent();

protected:
    CSharedPtr<CMesh>     m_pMesh;
    bool                    m_YRotEnable;
    Vector3                 m_OriginDir;

public:
    void EnableYBillboard()
    {
        m_YRotEnable = true;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(float DeltaTime);
    virtual CBillboardComponent* Clone();
};

