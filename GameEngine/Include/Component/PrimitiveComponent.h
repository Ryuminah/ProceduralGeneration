#pragma once

#include "SceneComponent.h"
#include "../Resource/Material.h"

class CPrimitiveComponent :
    public CSceneComponent
{
    friend class CGameObject;

protected:
    CPrimitiveComponent();
    CPrimitiveComponent(const CPrimitiveComponent& com);
    virtual ~CPrimitiveComponent();

protected:
    PrimitiveComponent_Type m_PrimitiveType;
    Render_Type_2D      m_2DType;
    Render_Type_3D      m_3DType;
    std::vector<CSharedPtr<CMaterial>>   m_vecMaterialSlot;
    bool        m_DistortionEnable;

public:
    bool GetDistortionEnable()  const
    {
        return m_DistortionEnable;
    }

    CMaterial* GetMaterial(int SlotIndex)   const
    {
        return m_vecMaterialSlot[SlotIndex].Get();
    }

    int GetMaterialCount()  const
    {
        return (int)m_vecMaterialSlot.size();
    }

    PrimitiveComponent_Type GetPrimitiveType()  const
    {
        return m_PrimitiveType;
    }

    Render_Type_2D GetRender2DType()  const
    {
        return m_2DType;
    }

    Render_Type_3D GetRender3DType()  const
    {
        return m_3DType;
    }

    void SetRender2DType(Render_Type_2D Type)
    {
        m_2DType = Type;
    }

    void SetRender3DType(Render_Type_3D Type)
    {
        m_3DType = Type;
    }

    void SetDistortion(bool Enable)
    {
        m_DistortionEnable = Enable;
    }

public:
    virtual void AddChild(CSceneComponent* Child, const std::string& SocketName = "");
    virtual void SetMaterial(int SlotIndex, CMaterial* pMaterial);
    virtual void SetMaterial(int SlotIndex, const std::string& Name);
    virtual void AddMaterial(CMaterial* pMaterial);
    virtual void AddMaterial(const std::string& Name);

public:
    virtual class CMesh* GetMesh()  const;
    virtual void SetMesh(class CMesh* pMesh);
    virtual void SetMesh(const std::string& Name);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostTransformUpdate(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void PrevRender(float DeltaTime);
    virtual void Render(float DeltaTime);
    virtual void RenderDebug(float DeltaTime);
    virtual CPrimitiveComponent* Clone();
    virtual void RenderShadow(float DeltaTime);
};

