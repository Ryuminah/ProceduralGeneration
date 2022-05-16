
#include "PrimitiveComponent.h"
#include "../Render/RenderManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "Transform.h"

CPrimitiveComponent::CPrimitiveComponent()
{
    m_SceneComponentType = SceneComponent_Type::Primitive;
    m_PrimitiveType = PrimitiveComponent_Type::Primitive3D;

    m_2DType = RT2D_None;
    m_3DType = RT3D_None;

    m_pTransform->SetDefaultZ(0.3f);

    m_DistortionEnable = false;
}

CPrimitiveComponent::CPrimitiveComponent(const CPrimitiveComponent& com)    :
    CSceneComponent(com)
{
    m_PrimitiveType = com.m_PrimitiveType;
    m_2DType = com.m_2DType;
    m_3DType = com.m_3DType;
    m_vecMaterialSlot = com.m_vecMaterialSlot;
    m_DistortionEnable = com.m_DistortionEnable;
}

CPrimitiveComponent::~CPrimitiveComponent()
{
}

void CPrimitiveComponent::SetMaterial(int SlotIndex, CMaterial* pMaterial)
{
    m_vecMaterialSlot[SlotIndex] = pMaterial;
}

void CPrimitiveComponent::SetMaterial(int SlotIndex, const std::string& Name)
{
    m_vecMaterialSlot[SlotIndex] = m_pScene->GetResource()->FindMaterial(Name);
}

void CPrimitiveComponent::AddMaterial(CMaterial* pMaterial)
{
    m_vecMaterialSlot.push_back(pMaterial);
}

void CPrimitiveComponent::AddMaterial(const std::string& Name)
{
    CMaterial* pMaterial = m_pScene->GetResource()->FindMaterial(Name);
    m_vecMaterialSlot.push_back(pMaterial);
}

CMesh* CPrimitiveComponent::GetMesh() const
{
    return nullptr;
}

void CPrimitiveComponent::SetMesh(CMesh* pMesh)
{
}

void CPrimitiveComponent::SetMesh(const std::string& Name)
{
}

void CPrimitiveComponent::Start()
{
    CSceneComponent::Start();
}

bool CPrimitiveComponent::Init()
{
    if (!CSceneComponent::Init())
        return false;

    return true;
}

void CPrimitiveComponent::Update(float DeltaTime)
{
    CSceneComponent::Update(DeltaTime);
}

void CPrimitiveComponent::PostUpdate(float DeltaTime)
{
    CSceneComponent::PostUpdate(DeltaTime);
}

void CPrimitiveComponent::Collision(float DeltaTime)
{
    CSceneComponent::Collision(DeltaTime);
}

void CPrimitiveComponent::PrevRender(float DeltaTime)
{
    CSceneComponent::PrevRender(DeltaTime);

    CRenderManager::GetInst()->AddPrimitiveComponent(this);
}

void CPrimitiveComponent::Render(float DeltaTime)
{
    CSceneComponent::Render(DeltaTime);
}

CPrimitiveComponent* CPrimitiveComponent::Clone()
{
    return new CPrimitiveComponent(*this);
}
