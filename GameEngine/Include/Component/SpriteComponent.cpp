
#include "SpriteComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "Transform.h"
#include "../Resource/Material.h"

CSpriteComponent::CSpriteComponent()
{
	m_PrimitiveType = PrimitiveComponent_Type::Primitive2D;
	m_2DType = RT2D_Default;
}

CSpriteComponent::CSpriteComponent(const CSpriteComponent& com) :
	CPrimitiveComponent(com)
{
	m_pMesh = com.m_pMesh;

	if (com.m_Animation)
	{
		m_Animation = com.m_Animation->Clone();
	}
}

CSpriteComponent::~CSpriteComponent()
{
}

CAnimation2D* CSpriteComponent::GetAnimation2D() const
{
	return m_Animation;
}

CMesh* CSpriteComponent::GetMesh() const
{
	return (CMesh*)m_pMesh.Get();
}

void CSpriteComponent::SetMesh(CMesh* pMesh)
{
	m_pMesh = (CSpriteMesh*)pMesh;

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

void CSpriteComponent::SetMesh(const std::string& Name)
{
	m_pMesh = (CSpriteMesh*)m_pScene->GetResource()->FindMesh(Name);

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

void CSpriteComponent::Start()
{
	CPrimitiveComponent::Start();

	if (m_Animation)
		m_Animation->Start();
}

bool CSpriteComponent::Init()
{
	if (!CPrimitiveComponent::Init())
		return false;

	//m_pMesh = (CSpriteMesh*)m_pScene->GetResource()->FindMesh("ColorRect");
	SetMesh("TextureRect");

	SetUpdatePosZ(true);

	return true;
}

void CSpriteComponent::Update(float DeltaTime)
{
	CPrimitiveComponent::Update(DeltaTime);

	if (m_Animation)
		m_Animation->Update(DeltaTime);
}

void CSpriteComponent::PostUpdate(float DeltaTime)
{
	CPrimitiveComponent::PostUpdate(DeltaTime);
}

void CSpriteComponent::Collision(float DeltaTime)
{
	CPrimitiveComponent::Collision(DeltaTime);
}

void CSpriteComponent::Render(float DeltaTime)
{
	CPrimitiveComponent::Render(DeltaTime);

	for (int i = 0; i < (int)m_vecMaterialSlot.size(); ++i)
	{
		m_vecMaterialSlot[i]->SetMaterial();

		if (m_Animation)
			m_Animation->SetShader();

		m_pMesh->Render(i);

		if (m_Animation)
			m_Animation->ResetShader();

		m_vecMaterialSlot[i]->ResetMaterial();
	}
}

CSpriteComponent* CSpriteComponent::Clone()
{
	return new CSpriteComponent(*this);
}

void CSpriteComponent::CallNotify(const std::string& Name)
{
	m_pOwner->Animation2DNotify(Name);
}
