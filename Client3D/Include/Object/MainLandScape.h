#pragma once

#include "GameObject.h"
#include "Component/LandScapeComponent.h"

class CMainLandScape :
    public CGameObject
{
    friend class CScene;

protected:
    CMainLandScape();
    CMainLandScape(const CMainLandScape& obj);
    virtual ~CMainLandScape();

protected:
    CSharedPtr<CLandScapeComponent> m_LandScape;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(float DeltaTime);
    virtual CMainLandScape* Clone();
};

