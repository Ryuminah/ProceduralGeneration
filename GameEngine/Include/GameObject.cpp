#include "GameObject.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"

CGameObject::CGameObject()  :
	m_Start(false),
	m_pScene(nullptr),
	m_LifeTime(0.f),
	m_LifeTimeEnable(false)
{
	m_RootComponent = CreateSceneComponent<CSceneComponent>("DefaultRoot");

	//m_SceneComponentList.push_back(m_RootComponent);
}

CGameObject::CGameObject(const CGameObject& obj)
{
	*this = obj;

	m_RefCount = 1;
}

CGameObject::~CGameObject()
{
	auto    iter = m_SceneComponentList.begin();
	auto    iterEnd = m_SceneComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_RELEASE((*iter));
	}

	m_SceneComponentList.clear();
}

void CGameObject::Active(bool bActive)
{
	CRef::Active(bActive);

	if (m_RootComponent)
		m_RootComponent->Active(bActive);
}

void CGameObject::SetRootComponent(CSceneComponent* Root)
{
	auto	iter = m_SceneComponentList.begin();
	auto	iterEnd = m_SceneComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == m_RootComponent)
		{
			SAFE_RELEASE((*iter));
			m_SceneComponentList.erase(iter);
			break;
		}
	}

	m_RootComponent = Root;
}

CSceneComponent* CGameObject::FindSceneComponent(const std::string& Name)
{
	if (m_RootComponent == nullptr)
		return nullptr;

	return m_RootComponent->FindComponent(Name);
}

void CGameObject::GetAllSceneComponentName(std::vector<HierarchyName>& vecName)
{
	if (!m_RootComponent)
		return;

	m_RootComponent->GetAllComponentName(vecName);
}

void CGameObject::GetAllSceneComponent(std::vector<CSceneComponent*>& vecComponent)
{
	if (!m_RootComponent)
		return;

	m_RootComponent->GetAllComponent(vecComponent);
}

CObjectComponent* CGameObject::FindObjectComponent(const std::string& Name)
{
	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecObjectComponent[i]->GetName() == Name)
			return m_vecObjectComponent[i];
	}

	return nullptr;
}

void CGameObject::GetAllObjectComponentName(std::vector<std::string>& vecName)
{
	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		vecName.push_back(m_vecObjectComponent[i]->GetName());
	}
}

void CGameObject::GetAllObjectComponent(std::vector<CObjectComponent*>& vecComponent)
{
	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		vecComponent.push_back(m_vecObjectComponent[i]);
	}
}

void CGameObject::Start()
{
	m_Start = true;

	m_RootComponent->Start();
}

bool CGameObject::Init()
{
	// 기본 RootComponent 하나를 무조건 만들어준다.
	//m_RootComponent = CreateSceneComponent<CSceneComponent>("DefaultRoot");
	//m_RootComponent->Release();

	return true;
}

void CGameObject::Update(float DeltaTime)
{
	if (m_LifeTimeEnable)
	{
		m_LifeTime -= DeltaTime;

		if (m_LifeTime <= 0.f)
			Active(false);
	}

	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Update(DeltaTime);
	}

	m_RootComponent->Update(DeltaTime);
}

void CGameObject::PostUpdate(float DeltaTime)
{
	m_RootComponent->PostUpdate(DeltaTime);

	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->PostUpdate(DeltaTime);
	}
}

void CGameObject::Collision(float DeltaTime)
{
	m_RootComponent->Collision(DeltaTime);

	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Collision(DeltaTime);
	}
}

void CGameObject::PrevRender(float DeltaTime)
{
	m_RootComponent->PrevRender(DeltaTime);

	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->PrevRender(DeltaTime);
	}
}

void CGameObject::Render(float DeltaTime)
{
	m_RootComponent->Render(DeltaTime);

	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Render(DeltaTime);
	}
}

CGameObject* CGameObject::Clone()
{
	return new CGameObject(*this);
}

void CGameObject::Animation2DNotify(const std::string& Name)
{
}

void CGameObject::AnimationNotify(const std::string& Name)
{
}

void CGameObject::ClearTransformState()
{
	auto    iter = m_SceneComponentList.begin();
	auto    iterEnd = m_SceneComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->ClearTransformState();
	}
}

void CGameObject::Move(const Vector2& Target)
{
	m_RootComponent->Move(Target);
}

void CGameObject::Move(const Vector3& Target)
{
	m_RootComponent->Move(Target);
}

Transform_State CGameObject::GetTransformState() const
{
	return m_RootComponent->GetTransformState();
}

void CGameObject::SetTransformState(Transform_State State)
{
	m_RootComponent->SetTransformState(State);
}


Vector3 CGameObject::GetVelocityScale() const
{
	return m_RootComponent.Get()->GetVelocityScale();
}

Vector3 CGameObject::GetVelocityRot() const
{
	return m_RootComponent.Get()->GetVelocityRot();
}

Vector3 CGameObject::GetVelocity() const
{
	return m_RootComponent.Get()->GetVelocity();
}

Vector3 CGameObject::GetAxis(AXIS Axis) const
{
	return m_RootComponent.Get()->GetAxis(Axis);
}

void CGameObject::SetInheritScale(bool Inherit)
{
	m_RootComponent->SetInheritScale(Inherit);
}

void CGameObject::SetInheritRotX(bool Inherit)
{
	m_RootComponent->SetInheritRotX(Inherit);
}

void CGameObject::SetInheritRotY(bool Inherit)
{
	m_RootComponent->SetInheritRotY(Inherit);
}

void CGameObject::SetInheritRotZ(bool Inherit)
{
	m_RootComponent->SetInheritRotZ(Inherit);
}

void CGameObject::SetInheritPosZ(bool Inherit)
{
	m_RootComponent->SetInheritPosZ(Inherit);
}

void CGameObject::SetUpdatePosZ(bool UpdatePosZ)
{
	m_RootComponent->SetUpdatePosZ(UpdatePosZ);
}

void CGameObject::InheritScale()
{
	m_RootComponent->InheritScale();
}

void CGameObject::InheritRot()
{
	m_RootComponent->InheritRot();
}

void CGameObject::InheritPos()
{
	m_RootComponent->InheritPos();
}

bool CGameObject::GetInheritPosZ() const
{
	return m_RootComponent->GetInheritPosZ();
}

Vector3 CGameObject::GetRelativeScale() const
{
	return m_RootComponent.Get()->GetRelativeScale();
}

Vector3 CGameObject::GetRelativeRotation() const
{
	return m_RootComponent.Get()->GetRelativeRotation();
}

Vector3 CGameObject::GetRelativePos() const
{
	return m_RootComponent.Get()->GetRelativePos();
}

void CGameObject::SetRelativeScale(const Vector3& Scale)
{
	m_RootComponent->SetRelativeScale(Scale);
}

void CGameObject::SetRelativeScale(float x, float y, float z)
{
	m_RootComponent->SetRelativeScale(x, y, z);
}

void CGameObject::SetRelativeRotation(const Vector3& Rot)
{
	m_RootComponent->SetRelativeRotation(Rot);
}

void CGameObject::SetRelativeRotation(float x, float y, float z)
{
	m_RootComponent->SetRelativeRotation(x, y, z);
}

void CGameObject::SetRelativeRotationX(float x)
{
	m_RootComponent->SetRelativeRotationX(x);
}

void CGameObject::SetRelativeRotationY(float y)
{
	m_RootComponent->SetRelativeRotationY(y);
}

void CGameObject::SetRelativeRotationZ(float z)
{
	m_RootComponent->SetRelativeRotationZ(z);
}

void CGameObject::SetRelativePos(const Vector3& Pos)
{
	m_RootComponent->SetRelativePos(Pos);
}

void CGameObject::SetRelativePos(float x, float y, float z)
{
	m_RootComponent->SetRelativePos(x, y, z);
}

void CGameObject::AddRelativeScale(const Vector3& Scale)
{
	m_RootComponent->AddRelativeScale(Scale);
}

void CGameObject::AddRelativeScale(float x, float y, float z)
{
	m_RootComponent->AddRelativeScale(x, y, z);
}

void CGameObject::AddRelativeRotation(const Vector3& Rot)
{
	m_RootComponent->AddRelativeRotation(Rot);
}

void CGameObject::AddRelativeRotation(float x, float y, float z)
{
	m_RootComponent->AddRelativeRotation(x, y, z);
}

void CGameObject::AddRelativeRotationX(float x)
{
	m_RootComponent->AddRelativeRotationX(x);
}

void CGameObject::AddRelativeRotationY(float y)
{
	m_RootComponent->AddRelativeRotationY(y);
}

void CGameObject::AddRelativeRotationZ(float z)
{
	m_RootComponent->AddRelativeRotationZ(z);
}

void CGameObject::AddRelativePos(const Vector3& Pos)
{
	m_RootComponent->AddRelativePos(Pos);
}

void CGameObject::AddRelativePos(float x, float y, float z)
{
	m_RootComponent->AddRelativePos(x, y, z);
}

Vector3 CGameObject::GetWorldScale() const
{
	return m_RootComponent.Get()->GetWorldScale();
}

Vector3 CGameObject::GetWorldRotation() const
{
	return m_RootComponent.Get()->GetWorldRotation();
}

Vector3 CGameObject::GetWorldPos() const
{
	return m_RootComponent.Get()->GetWorldPos();
}

Vector3 CGameObject::GetPrevWorldPos() const
{
	return m_RootComponent.Get()->GetPrevWorldPos();
}

Vector3 CGameObject::GetPivot() const
{
	return m_RootComponent.Get()->GetPivot();
}

Vector3 CGameObject::GetMeshSize() const
{
	return m_RootComponent.Get()->GetMeshSize();
}

Matrix CGameObject::GetScaleMatrix() const
{
	return m_RootComponent.Get()->GetScaleMatrix();
}

Matrix CGameObject::GetRotatinMatrix() const
{
	return m_RootComponent.Get()->GetRotatinMatrix();
}

Matrix CGameObject::GetTranslationMatrix() const
{
	return m_RootComponent.Get()->GetTranslationMatrix();
}

Matrix CGameObject::GetWorldMatrix() const
{
	return m_RootComponent.Get()->GetWorldMatrix();
}

void CGameObject::SetPivot(const Vector3& Pivot)
{
	m_RootComponent->SetPivot(Pivot);
}

void CGameObject::SetPivot(float x, float y, float z)
{
	m_RootComponent->SetPivot(x, y, z);
}

void CGameObject::SetMeshSize(const Vector3& Size)
{
	m_RootComponent->SetMeshSize(Size);
}

void CGameObject::SetMeshSize(float x, float y, float z)
{
	m_RootComponent->SetMeshSize(x, y, z);
}

void CGameObject::SetWorldScale(const Vector3& Scale)
{
	m_RootComponent->SetWorldScale(Scale);
}

void CGameObject::SetWorldScale(float x, float y, float z)
{
	m_RootComponent->SetWorldScale(x, y, z);
}

void CGameObject::SetWorldRotation(const Vector3& Rot)
{
	m_RootComponent->SetWorldRotation(Rot);
}

void CGameObject::SetWorldRotation(float x, float y, float z)
{
	m_RootComponent->SetWorldRotation(x, y, z);
}

void CGameObject::SetWorldRotationX(float x)
{
	m_RootComponent->SetWorldRotationX(x);
}

void CGameObject::SetWorldRotationY(float y)
{
	m_RootComponent->SetWorldRotationY(y);
}

void CGameObject::SetWorldRotationZ(float z)
{
	m_RootComponent->SetWorldRotationZ(z);
}

void CGameObject::SetWorldPos(const Vector3& Pos)
{
	m_RootComponent->SetWorldPos(Pos);
}

void CGameObject::SetWorldPos(float x, float y, float z)
{
	m_RootComponent->SetWorldPos(x, y, z);
}

void CGameObject::AddWorldScale(const Vector3& Scale)
{
	m_RootComponent->AddWorldScale(Scale);
}

void CGameObject::AddWorldScale(float x, float y, float z)
{
	m_RootComponent->AddWorldScale(x, y, z);
}

void CGameObject::AddWorldRotation(const Vector3& Rot)
{
	m_RootComponent->AddWorldRotation(Rot);
}

void CGameObject::AddWorldRotation(float x, float y, float z)
{
	m_RootComponent->AddWorldRotation(x, y, z);
}

void CGameObject::AddWorldRotationX(float x)
{
	m_RootComponent->AddWorldRotationX(x);
}

void CGameObject::AddWorldRotationY(float y)
{
	m_RootComponent->AddWorldRotationY(y);
}

void CGameObject::AddWorldRotationZ(float z)
{
	m_RootComponent->AddWorldRotationZ(z);
}

void CGameObject::AddWorldPos(const Vector3& Pos)
{
	m_RootComponent->AddWorldPos(Pos);
}

void CGameObject::AddWorldPos(float x, float y, float z)
{
	m_RootComponent->AddWorldPos(x, y, z);
}

void CGameObject::AddCamera(CCamera* Camera)
{
	m_pScene->GetCameraManager()->AddCamera(Camera);
}

void CGameObject::LookAt(const Vector3& Pos)
{
	m_RootComponent->LookAt(Pos);
}

void CGameObject::LookAt(const Vector3& Pos, const Vector3& OriginDir)
{
	m_RootComponent->LookAt(Pos, OriginDir);
}

void CGameObject::LookAtYAxis(const Vector3& Pos)
{
	m_RootComponent->LookAtYAxis(Pos);
}

void CGameObject::LookAtYAxis(const Vector3& Pos, const Vector3& OriginDir)
{
	m_RootComponent->LookAtYAxis(Pos, OriginDir);
}
