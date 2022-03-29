#pragma once

#include "ObjectComponent.h"
#include "../Resource/Material.h"
#include "PrimitiveComponent.h"

class CDistortionComponent :
    public CObjectComponent
{
	friend class CGameObject;

protected:
	CDistortionComponent();
	CDistortionComponent(const CDistortionComponent& com);
	virtual ~CDistortionComponent();

protected:
	class CDistortionCBuffer* m_CBuffer;
	std::vector<CSharedPtr<CMaterial>>	m_vecTargetMaterial;
	bool		m_DistortionEnable;
	CSharedPtr<CPrimitiveComponent> m_TargetPrimitive;

public:
	void SetTargetMaterial(CMaterial* Mtrl)
	{
		m_vecTargetMaterial.push_back(Mtrl);

		Mtrl->AddSetMaterialCallback<CDistortionComponent>("Distortion",
			this, &CDistortionComponent::SetMaterialCallback);
		Mtrl->AddResetMaterialCallback<CDistortionComponent>("Distortion",
			this, &CDistortionComponent::ResetMaterialCallback);
	}

	void SetTargetPrimitive(CPrimitiveComponent* Primitive);

	void SetWeight(float Weight);
	void SetSpeed(float Speed);
	void EnableDistortion(bool Enable);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CDistortionComponent* Clone();

public:
	void SetMaterialCallback();
	void ResetMaterialCallback();
};

