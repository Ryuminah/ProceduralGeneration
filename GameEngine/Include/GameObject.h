#pragma once

#include "Ref.h"
#include "Component/SceneComponent.h"
#include "Component/Camera.h"
#include "Component/ObjectComponent.h"

class CGameObject :
	public CRef
{
	friend class CScene;

protected:
	CGameObject();
	CGameObject(const CGameObject& obj);
	virtual ~CGameObject();

protected:
	class CScene* m_pScene;
	bool            m_Start;
	float			m_LifeTime;
	bool			m_LifeTimeEnable;

public:
	void SetLifeTime(float Time)
	{
		if (Time > 0.f)
			m_LifeTimeEnable = true;

		m_LifeTime = Time;
	}

public:
	bool IsStart()  const
	{
		return m_Start;
	}

protected:
	CSharedPtr<CSceneComponent> m_RootComponent;
	std::list<CSceneComponent*> m_SceneComponentList;
	std::vector<CSharedPtr<CObjectComponent>>	m_vecObjectComponent;

public:
	void GetComponentName(std::vector<std::string>& vecName)
	{
		auto	iter = m_SceneComponentList.begin();
		auto	iterEnd = m_SceneComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			vecName.push_back((*iter)->GetName());
		}
	}

public:
	CSceneComponent* GetRootComponent()   const
	{
		return m_RootComponent.Get();
	}

	virtual void Active(bool bActive);

	void SetRootComponent(CSceneComponent* Root);
	CSceneComponent* FindSceneComponent(const std::string& Name);
	void GetAllSceneComponentName(std::vector<HierarchyName>& vecName);
	void GetAllSceneComponent(std::vector<CSceneComponent*>& vecComponent);
	CObjectComponent* FindObjectComponent(const std::string& Name);
	void GetAllObjectComponentName(std::vector<std::string>& vecName);
	void GetAllObjectComponent(std::vector<CObjectComponent*>& vecComponent);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CGameObject* Clone();
	virtual void Animation2DNotify(const std::string& Name);
	virtual void AnimationNotify(const std::string& Name);
	void ClearTransformState();

public:
	void Move(const Vector2& Target);
	void Move(const Vector3& Target);

public:
	Transform_State GetTransformState() const;
	void SetTransformState(Transform_State State);
	Vector3 GetVelocityScale()  const;
	Vector3 GetVelocityRot()    const;
	Vector3 GetVelocity()   const;
	Vector3 GetAxis(AXIS Axis)  const;

public:
	void SetInheritScale(bool Inherit);
	void SetInheritRotX(bool Inherit);
	void SetInheritRotY(bool Inherit);
	void SetInheritRotZ(bool Inherit);
	void SetInheritPosZ(bool Inherit);
	void SetUpdatePosZ(bool UpdatePosZ);
	void InheritScale();
	void InheritRot();
	void InheritPos();

public:
	bool GetInheritPosZ()	const;

public:
	Vector3 GetRelativeScale()  const;
	Vector3 GetRelativeRotation()    const;
	Vector3 GetRelativePos()    const;

public:
	void SetRelativeScale(const Vector3& Scale);
	void SetRelativeScale(float x, float y, float z);
	void SetRelativeRotation(const Vector3& Rot);
	void SetRelativeRotation(float x, float y, float z);
	void SetRelativeRotationX(float x);
	void SetRelativeRotationY(float y);
	void SetRelativeRotationZ(float z);
	void SetRelativePos(const Vector3& Pos);
	void SetRelativePos(float x, float y, float z);
	void AddRelativeScale(const Vector3& Scale);
	void AddRelativeScale(float x, float y, float z);
	void AddRelativeRotation(const Vector3& Rot);
	void AddRelativeRotation(float x, float y, float z);
	void AddRelativeRotationX(float x);
	void AddRelativeRotationY(float y);
	void AddRelativeRotationZ(float z);
	void AddRelativePos(const Vector3& Pos);
	void AddRelativePos(float x, float y, float z);

public:
	Vector3 GetWorldScale() const;
	Vector3 GetWorldRotation()  const;
	Vector3 GetWorldPos()   const;
	Vector3 GetPrevWorldPos()   const;

	Vector3 GetPivot()  const;
	Vector3 GetMeshSize()   const;
	Matrix GetScaleMatrix() const;
	Matrix GetRotatinMatrix()   const;
	Matrix GetTranslationMatrix()   const;
	Matrix GetWorldMatrix() const;

public:
	void SetPivot(const Vector3& Pivot);
	void SetPivot(float x, float y, float z);
	void SetMeshSize(const Vector3& Size);
	void SetMeshSize(float x, float y, float z);

public:
	void SetWorldScale(const Vector3& Scale);
	void SetWorldScale(float x, float y, float z);
	void SetWorldRotation(const Vector3& Rot);
	void SetWorldRotation(float x, float y, float z);
	void SetWorldRotationX(float x);
	void SetWorldRotationY(float y);
	void SetWorldRotationZ(float z);
	void SetWorldPos(const Vector3& Pos);
	void SetWorldPos(float x, float y, float z);
	void AddWorldScale(const Vector3& Scale);
	void AddWorldScale(float x, float y, float z);
	void AddWorldRotation(const Vector3& Rot);
	void AddWorldRotation(float x, float y, float z);
	void AddWorldRotationX(float x);
	void AddWorldRotationY(float y);
	void AddWorldRotationZ(float z);
	void AddWorldPos(const Vector3& Pos);
	void AddWorldPos(float x, float y, float z);

public:
	void LookAt(const Vector3& Pos);
	void LookAt(const Vector3& Pos, const Vector3& OriginDir);
	void LookAtYAxis(const Vector3& Pos);
	void LookAtYAxis(const Vector3& Pos, const Vector3& OriginDir);

private:
	void AddCamera(CCamera* Camera);

public:
	template <typename T>
	T* CreateSceneComponent(const std::string& Name)
	{
		T* pComponent = new T;

		pComponent->SetName(Name);
		pComponent->m_pScene = m_pScene;
		pComponent->m_pOwner = this;

		if (!pComponent->Init())
		{
			SAFE_RELEASE(pComponent);
			return nullptr;
		}

		m_SceneComponentList.push_back(pComponent);

		if (typeid(T).hash_code() == typeid(CCamera).hash_code())
			AddCamera((CCamera*)pComponent);

		return pComponent;
	}

	template <typename T>
	T* CreateObjectComponent(const std::string& Name)
	{
		T* pComponent = new T;

		pComponent->SetName(Name);
		pComponent->m_pScene = m_pScene;
		pComponent->m_pOwner = this;

		if (!pComponent->Init())
		{
			SAFE_RELEASE(pComponent);
			return nullptr;
		}

		m_vecObjectComponent.push_back(pComponent);

		pComponent->Release();

		return pComponent;
	}
};

