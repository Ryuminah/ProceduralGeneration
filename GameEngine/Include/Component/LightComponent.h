#pragma once

#include "SceneComponent.h"
#include "../Resource/LightConstantBuffer.h"

class CLightComponent :
    public CSceneComponent
{
	friend class CGameObject;
	friend class CCameraManager;
	friend class CViewport;

protected:
	CLightComponent();
	CLightComponent(const CLightComponent& com);
	virtual ~CLightComponent();

protected:
	CLightConstantBuffer*	m_CBuffer;

public:
	Light_Type GetLightType()	const
	{
		return (Light_Type)m_CBuffer->GetLightType();
	}

	float GetLightDistance()	const
	{
		return m_CBuffer->GetLightDistance();
	}

	void SetLightType(Light_Type Type)
	{
		m_CBuffer->SetLightType(Type);
	}

	void SetDiffuse(const Vector4& Color)
	{
		m_CBuffer->SetDiffuse(Color);
	}

	void SetAmbient(const Vector4& Color)
	{
		m_CBuffer->SetAmbient(Color);
	}

	void SetSpecular(const Vector4& Color)
	{
		m_CBuffer->SetSpecular(Color);
	}

	void SetPos(const Vector3& Pos)
	{
		m_CBuffer->SetPos(Pos);
	}

	void SetDir(const Vector3& Dir)
	{
		m_CBuffer->SetDir(Dir);
	}

	void SetDistance(float Distance)
	{
		m_CBuffer->SetDistance(Distance);
	}

	void SetAngleIn(float AngleIn)
	{
		m_CBuffer->SetAngleIn(AngleIn);
	}

	void SetAngleOut(float AngleOut)
	{
		m_CBuffer->SetAngleOut(AngleOut);
	}

	void SetAtt1(float Att1)
	{
		m_CBuffer->SetAtt1(Att1);
	}

	void SetAtt2(float Att2)
	{
		m_CBuffer->SetAtt2(Att2);
	}

	void SetAtt3(float Att3)
	{
		m_CBuffer->SetAtt3(Att3);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CLightComponent* Clone();

public:
	void SetShader();
};

