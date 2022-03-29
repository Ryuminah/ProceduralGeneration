#pragma once

#include "PrimitiveComponent.h"

enum class Decal_Fade_State
{
	FadeIn,
	FadeOut,
	Duration
};

class CDecalComponent :
	public CPrimitiveComponent
{
	friend class CGameObject;

protected:
	CDecalComponent();
	CDecalComponent(const CDecalComponent& com);
	virtual ~CDecalComponent();

protected:
	CSharedPtr<CMesh>     m_pMesh;
	CSharedPtr<CMesh>     m_pDebugMesh;
	CSharedPtr<CMaterial>   m_DebugMaterial;
	Decal_Fade_State		m_FadeState;

protected:
	float       m_FadeInTime;
	float		m_FadeIn;

	float		m_FadeOutTime;
	float		m_FadeOut;

	float		m_DurationTime;
	float		m_Duration;

	bool		m_FadeLoop;

public:
	void SetFadeInTime(float Time)
	{
		m_FadeInTime = Time;
	}

	void SetFadeOutTime(float Time)
	{
		m_FadeOutTime = Time;
	}

	void SetDuration(float Time)
	{
		m_DurationTime = Time;
	}

	void SetFadeLoop(bool Loop)
	{
		m_FadeLoop = Loop;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual void RenderDebug(float DeltaTime);
	virtual CDecalComponent* Clone();
};

