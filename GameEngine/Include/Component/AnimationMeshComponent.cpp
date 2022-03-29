
#include "AnimationMeshComponent.h"
#include "../Resource/Animation.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "Transform.h"
#include "../Resource/Material.h"
#include "../Resource/AnimationMesh.h"

CAnimationMeshComponent::CAnimationMeshComponent()
{
	m_PrimitiveType = PrimitiveComponent_Type::Primitive3D;
	m_3DType = RT3D_Default;
	m_Animation = nullptr;
	m_Skeleton = nullptr;
}

CAnimationMeshComponent::CAnimationMeshComponent(const CAnimationMeshComponent& com) :
	CPrimitiveComponent(com)
{
	m_pMesh = com.m_pMesh;

	if (com.m_Animation)
		m_Animation = com.m_Animation->Clone();

	m_Skeleton = com.m_Skeleton;

	if (m_Skeleton)
	{
		m_BoneSocket = m_Skeleton->GetSocket(m_SocketName);

		m_pTransform->SetSocket(m_BoneSocket);
	}
}

CAnimationMeshComponent::~CAnimationMeshComponent()
{
	SAFE_DELETE(m_Animation);
}

void CAnimationMeshComponent::AddChild(CSceneComponent* Child, const std::string& SocketName)
{
	CPrimitiveComponent::AddChild(Child, SocketName);

	if (m_Skeleton)
	{
		m_BoneSocket = m_Skeleton->GetSocket(m_SocketName);

		m_pTransform->SetSocket(m_BoneSocket);
	}
}

void CAnimationMeshComponent::SetAnimation(CAnimation* Animation)
{
	m_Animation = Animation;

	if (m_Animation)
	{
		m_Animation->SetScene(m_pScene);
		m_Animation->SetOwner(this);

		m_Animation->SetSkeleton(m_Skeleton);
	}
}

CMesh* CAnimationMeshComponent::GetMesh() const
{
	return (CMesh*)m_pMesh.Get();
}

void CAnimationMeshComponent::SetMesh(CMesh* pMesh)
{
	m_pMesh = (CAnimationMesh*)pMesh;

	m_Skeleton = m_pMesh->GetSkeleton();

	if (m_Animation)
		m_Animation->SetSkeleton(m_Skeleton);

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

void CAnimationMeshComponent::SetMesh(const std::string& Name)
{
	m_pMesh = (CAnimationMesh*)m_pScene->GetResource()->FindMesh(Name);

	m_Skeleton = m_pMesh->GetSkeleton();

	if (m_Animation)
		m_Animation->SetSkeleton(m_Skeleton);

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

void CAnimationMeshComponent::Start()
{
	CPrimitiveComponent::Start();

	if (m_Animation)
		m_Animation->Start();

	if (m_Skeleton)
	{
		m_BoneSocket = m_Skeleton->GetSocket(m_SocketName);

		m_pTransform->SetSocket(m_BoneSocket);
	}
}

bool CAnimationMeshComponent::Init()
{
	if (!CPrimitiveComponent::Init())
		return false;

	return true;
}

void CAnimationMeshComponent::Update(float DeltaTime)
{
	CPrimitiveComponent::Update(DeltaTime);

	if (m_Animation)
		m_Animation->Update(DeltaTime);
}

void CAnimationMeshComponent::PostTransformUpdate(float DeltaTime)
{
	CPrimitiveComponent::PostTransformUpdate(DeltaTime);

	if (m_Animation)
		m_Animation->PostUpdate(DeltaTime);
}

void CAnimationMeshComponent::PostUpdate(float DeltaTime)
{
	CPrimitiveComponent::PostUpdate(DeltaTime);

}

void CAnimationMeshComponent::Collision(float DeltaTime)
{
	CPrimitiveComponent::Collision(DeltaTime);
}

void CAnimationMeshComponent::Render(float DeltaTime)
{
	CPrimitiveComponent::Render(DeltaTime);

	if (m_Animation)
		m_Animation->SetShader();

	for (int i = 0; i < (int)m_vecMaterialSlot.size(); ++i)
	{
		m_vecMaterialSlot[i]->SetMaterial();

		m_pMesh->Render(i);

		m_vecMaterialSlot[i]->ResetMaterial();
	}

	if (m_Animation)
		m_Animation->ResetShader();
}

CAnimationMeshComponent* CAnimationMeshComponent::Clone()
{
	return new CAnimationMeshComponent(*this);
}

void CAnimationMeshComponent::RenderShadow(float DeltaTime)
{
	CPrimitiveComponent::RenderShadow(DeltaTime);

	if (m_Animation)
		m_Animation->SetShader();

	for (int i = 0; i < (int)m_vecMaterialSlot.size(); ++i)
	{
		m_vecMaterialSlot[i]->SetShadowMaterial();

		m_pMesh->Render(i);

		m_vecMaterialSlot[i]->ResetShadowMaterial();
	}

	if (m_Animation)
		m_Animation->ResetShader();
}

void CAnimationMeshComponent::CallNotify(const std::string& Name)
{
	m_pOwner->AnimationNotify(Name);
}
