#pragma once
#include "ObjectComponent.h"
#include "../Resource/Material.h"
class CPaperBurnComponent :
	public CObjectComponent
{
	friend class CGameObject;

protected:
	CPaperBurnComponent();
	CPaperBurnComponent(const CPaperBurnComponent& com);
	virtual ~CPaperBurnComponent();

protected:
	class CPaperBurnCBuffer* m_CBuffer;
	std::vector<CSharedPtr<CMaterial>>	m_vecTargetMaterial;
	bool		m_PaperBurnEnable;
	bool		m_Inverse;
	float		m_BurnTimeMax;
	float		m_BurnTime;
	std::function<void()>	m_EndFunction;

public:
	void SetTargetMaterial(CMaterial* Mtrl)
	{
		m_vecTargetMaterial.push_back(Mtrl);

		Mtrl->AddSetMaterialCallback<CPaperBurnComponent>("PaperBurn",
			this, &CPaperBurnComponent::SetMaterialCallback);
		Mtrl->AddResetMaterialCallback<CPaperBurnComponent>("PaperBurn",
			this, &CPaperBurnComponent::ResetMaterialCallback);
	}

	void StartPaperBurn()
	{
		m_PaperBurnEnable = true;

		size_t	Size = m_vecTargetMaterial.size();

		for (size_t i = 0; i < Size; ++i)
		{
			m_vecTargetMaterial[i]->PaperBurnEnable(true);
		}

	}

	void SetBurnTime(float Time)
	{
		m_BurnTimeMax = Time;
	}

	void SetInLineColor(const Vector3& Color);
	void SetInLineColorUNorm(float r, float g, float b);
	void SetInLineColor(unsigned char r, unsigned char g, unsigned char b);

	void SetCenterLineColor(const Vector3& Color);
	void SetCenterLineColorUNorm(float r, float g, float b);
	void SetCenterLineColor(unsigned char r, unsigned char g, unsigned char b);

	void SetOutLineColor(const Vector3& Color);
	void SetOutLineColorUNorm(float r, float g, float b);
	void SetOutLineColor(unsigned char r, unsigned char g, unsigned char b);

	void SetInFilter(float Filter);
	void SetCenterFilter(float Filter);
	void SetOutFilter(float Filter);

	void Inverse();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CPaperBurnComponent* Clone();

public:
	template <typename T>
	void SetEndFunction(T* Obj, void(T::* Func)())
	{
		m_EndFunction = std::bind(Func, Obj);
	}

	void SetMaterialCallback();
	void ResetMaterialCallback();
};

