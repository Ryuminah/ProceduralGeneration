#pragma once

#include "PrimitiveComponent.h"
#include "../Resource/StaticMesh.h"

class CStaticMeshComponent :
    public CPrimitiveComponent
{
    friend class CGameObject;

protected:
    CStaticMeshComponent();
    CStaticMeshComponent(const CStaticMeshComponent& com);
    virtual ~CStaticMeshComponent();

protected:
    CSharedPtr<CStaticMesh>     m_pMesh;

public:
    virtual class CMesh* GetMesh()  const;
    virtual void SetMesh(class CMesh* pMesh);
    virtual void SetMesh(const std::string& Name);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(float DeltaTime);
    virtual CStaticMeshComponent* Clone();
    virtual void RenderShadow(float DeltaTime);
};

