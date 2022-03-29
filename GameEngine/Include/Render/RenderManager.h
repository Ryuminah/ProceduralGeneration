#pragma once

#include "../GameEngine.h"
#include "../Resource/RenderTarget.h"
#include "../Resource/Material.h"
#include "../UI/MouseWidget.h"

class CRenderManager
{
private:
    class CPostProcessCBuffer* m_PostProcessCBuffer;

    Render_Space    m_RenderSpace;

    class CPrimitiveComponent** m_pRenderList2D[RT2D_End];
    int  m_RenderList2DCount[RT2D_End];
    int  m_RenderList2DCapacity[RT2D_End];

    class CPrimitiveComponent** m_pRenderList3D[RT3D_End];
    int m_RenderList3DCount[RT3D_End];
    int m_RenderList3DCapacity[RT3D_End];

    class CPrimitiveComponent** m_pRenderListDistortion;
    int m_RenderListDistortionCount;
    int m_RenderListDistortionCapacity;

    class CRenderState* m_WorldState[RST_End];
    class CRenderState* m_DepthDisable;
    class CRenderState* m_DepthWriteDisable;
    class CRenderState* m_AlphaBlend;
    class CRenderState* m_UIAlphaBlend;
    class CRenderState* m_DecalBlend;
    class CRenderState* m_SkyState;
    class CRenderState* m_CullNone;

    std::vector<CSharedPtr<CRenderTarget>>  m_vecGBuffer;
    std::vector<CSharedPtr<CRenderTarget>>  m_vecDecal;
    std::vector<CSharedPtr<CRenderTarget>>  m_vecLight;
    class CRenderState* m_LightAccBlend;
    CSharedPtr<class CShader>   m_LightAccShader;

    CSharedPtr<CRenderTarget> m_LightCombineTarget;
    CSharedPtr<class CShader>   m_LightCombineShader;
    CSharedPtr<class CShader>   m_LightCombineRenderShader;

    CSharedPtr<CRenderTarget> m_ShadowTarget;
    CSharedPtr<class CShader>   m_ShadowMapShader;
    class CShadowCBuffer* m_ShadowCBuffer;
    float                   m_ShadowBias;
    float                   m_ShadowMapSize;

    // 그림자 맵용 깊이버퍼 생성
    ID3D11DepthStencilView* m_ShadowDepth;

    CSharedPtr<class CShader>   m_FullScreenShader;

    CSharedPtr<CRenderTarget> m_DiffuseTarget;
    CSharedPtr<CMaterial>   m_DiffuseTargetMaterial;

    CSharedPtr<CRenderTarget> m_DistortionTarget;
    CSharedPtr<CMaterial>   m_DistortionTargetMaterial;

    class CDistortionCBuffer* m_DistortionCBuffer;

    CSharedPtr<CMouseWidget>    m_MouseWidget;

public:
    void SetMouseWidget(CMouseWidget* Widget)
    {
        m_MouseWidget = Widget;
    }

    Render_Space GetRenderSpace()   const
    {
        return m_RenderSpace;
    }

    CRenderTarget* GetDiffuseTarget()   const
    {
        return m_DiffuseTarget;
    }

    void SetWorldRenderState(const std::string& Name);
    void SetBlur(bool Enable);
    void SetDistortion(bool Enable);
    void SetDistortionWeight(float Weight);
    void SetDistortionSpeed(float Speed);


public:
    bool Init();
    void SetDefaultTarget();
    void Render(float DeltaTime);

private:
    void Render2D(float DeltaTime);
    void Render3D(float DeltaTime);
    void RenderGBuffer(float DeltaTime);
    void RenderShadow(float DeltaTime);
    void RenderDecal(float DeltaTime);
    void RenderDecalDebug(float DeltaTime);
    void RenderLightAcc(float DeltaTime);
    void RenderLightCombine(float DeltaTime);
    void RenderCombineTarget(float DeltaTime);
    
public:
    void AddPrimitiveComponent(class CPrimitiveComponent* pPrimitive);

private:
    static bool Sort2DObject(class CPrimitiveComponent* Src, class CPrimitiveComponent* Dest);
    static bool Sort3DObject(class CPrimitiveComponent* Src, class CPrimitiveComponent* Dest);
    static bool Sort3DAlpha(class CPrimitiveComponent* Src, class CPrimitiveComponent* Dest);

    DECLARE_SINGLE(CRenderManager)
};

