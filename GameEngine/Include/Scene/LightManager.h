#pragma once

#include "../GameEngine.h"

class CLightManager
{
    friend class CScene;

protected:
    CLightManager();
    virtual ~CLightManager();

protected:
    class CScene* m_pScene;
    std::list<CSharedPtr<class CLightComponent>>    m_LightList;
    CSharedPtr<class CGameObject>   m_GlobalLight;
    CSharedPtr<class CLightComponent>   m_GlobalLightComponent;
    CSharedPtr<class CShader>		m_LightDirShader;
    CSharedPtr<class CShader>		m_LightShader;
    CSharedPtr<class CMesh>         m_SphereMesh;
    class CRenderState* m_DepthWriteDisable;
    class CRenderState* m_LightVolumeBackState;
    class CRenderState* m_LightVolumeFrontState;
    class CRenderState* m_FrontFaceCull;
    class CRenderState* m_CullNone;
    class CRenderState* m_LightAccState;

public:
    class CLightComponent* GetGlobalLightComponent()    const
    {
        return m_GlobalLightComponent;
    }

public:
    void AddLight(class CLightComponent* Light);

public:
    bool Init();
    void Update(float DeltaTime);
    void SetShader();
    void Render();
};

