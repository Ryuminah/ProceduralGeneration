#pragma once

#include "GameObject.h"
#include "Component/DecalComponent.h"

class CDecalTest :
    public CGameObject
{
    friend class CScene;

protected:
    CDecalTest();
    CDecalTest(const CDecalTest& obj);
    virtual ~CDecalTest();

protected:
    CSharedPtr<CDecalComponent> m_Decal;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(float DeltaTime);
    virtual CDecalTest* Clone();
};

