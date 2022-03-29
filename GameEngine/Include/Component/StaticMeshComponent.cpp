
#include "StaticMeshComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "Transform.h"
#include "../Resource/Material.h"

CStaticMeshComponent::CStaticMeshComponent()
{
	m_PrimitiveType = PrimitiveComponent_Type::Primitive3D;
	m_3DType = RT3D_Default;
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& com) :
	CPrimitiveComponent(com)
{
	m_pMesh = com.m_pMesh;
}

CStaticMeshComponent::~CStaticMeshComponent()
{
}

CMesh* CStaticMeshComponent::GetMesh() const
{
	return (CMesh*)m_pMesh.Get();
}

void CStaticMeshComponent::SetMesh(CMesh* pMesh)
{
	m_pMesh = (CStaticMesh*)pMesh;

	Vector3 Min = m_pMesh->GetMin();
	Vector3 Max = m_pMesh->GetMax();

	m_pTransform->SetMeshSize(Max - Min);

	m_vecMaterialSlot.clear();

	const std::vector<CSharedPtr<CMaterial>>* pMaterialSlots =
		m_pMesh->GetMaterialSlots();

	auto    iter = pMaterialSlots->begin();
	auto    iterEnd = pMaterialSlots->end();

	for (; iter != iterEnd; ++iter)
	{
		CMaterial* pClone = (*iter)->Clone();

		pClone->SetScene(m_pScene);
		m_vecMaterialSlot.push_back(pClone);

		SAFE_RELEASE(pClone);
	}
}

void CStaticMeshComponent::SetMesh(const std::string& Name)
{
	m_pMesh = (CStaticMesh*)m_pScene->GetResource()->FindMesh(Name);

	Vector3 Min = m_pMesh->GetMin();
	Vector3 Max = m_pMesh->GetMax();

	m_pTransform->SetMeshSize(Max - Min);

	m_vecMaterialSlot.clear();

	const std::vector<CSharedPtr<CMaterial>>* pMaterialSlots =
		m_pMesh->GetMaterialSlots();

	auto    iter = pMaterialSlots->begin();
	auto    iterEnd = pMaterialSlots->end();

	for (; iter != iterEnd; ++iter)
	{
		CMaterial* pClone = (*iter)->Clone();

		pClone->SetScene(m_pScene);
		m_vecMaterialSlot.push_back(pClone);

		SAFE_RELEASE(pClone);
	}
}

void CStaticMeshComponent::Start()
{
	CPrimitiveComponent::Start();
}

bool CStaticMeshComponent::Init()
{
	if (!CPrimitiveComponent::Init())
		return false;

	//m_pMesh = (CSpriteMesh*)m_pScene->GetResource()->FindMesh("ColorRect");
	//SetMesh("TextureRect");

	//SetUpdatePosZ(true);

	return true;
}

void CStaticMeshComponent::Update(float DeltaTime)
{
	CPrimitiveComponent::Update(DeltaTime);
}

void CStaticMeshComponent::PostUpdate(float DeltaTime)
{
	CPrimitiveComponent::PostUpdate(DeltaTime);
}

void CStaticMeshComponent::Collision(float DeltaTime)
{
	CPrimitiveComponent::Collision(DeltaTime);
}

void CStaticMeshComponent::Render(float DeltaTime)
{
	CPrimitiveComponent::Render(DeltaTime);

	for (int i = 0; i < (int)m_vecMaterialSlot.size(); ++i)
	{
		m_vecMaterialSlot[i]->SetMaterial();

		m_pMesh->Render(i);

		m_vecMaterialSlot[i]->ResetMaterial();
	}
}

CStaticMeshComponent* CStaticMeshComponent::Clone()
{
	return new CStaticMeshComponent(*this);
}

void CStaticMeshComponent::RenderShadow(float DeltaTime)
{
	CPrimitiveComponent::RenderShadow(DeltaTime);

	for (int i = 0; i < (int)m_vecMaterialSlot.size(); ++i)
	{
		m_vecMaterialSlot[i]->SetShadowMaterial();

		m_pMesh->Render(i);

		m_vecMaterialSlot[i]->ResetShadowMaterial();
	}
}
