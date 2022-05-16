#pragma once

#include "SceneComponent.h"

class CCamera :
	public CSceneComponent
{
	friend class CGameObject;
	friend class CCameraManager;
	friend class CViewport;

protected:
	CCamera();
	CCamera(const CCamera& com);
	virtual ~CCamera();

protected:
	Camera_Type m_CameraType;
	Matrix  m_matView;
	Matrix  m_matProj;
	float   m_ViewAngle;
	float   m_Distance;
	float	m_CameraBottom;	// 2D 전용
	float	m_CameraLeft;	// 2D 전용
	Resolution	m_RS;

public:
	Resolution GetResolution()	const
	{
		return m_RS;
	}

	float GetCameraBottom()	const
	{
		return m_CameraBottom;
	}

	float GetCameraLeft()	const
	{
		return m_CameraLeft;
	}

	Camera_Type GetCameraType() const
	{
		return m_CameraType;
	}

	Matrix GetViewMatrix()  const
	{
		return m_matView;
	}

	Matrix GetProjMatrix()  const
	{
		return m_matProj;
	}

public:
	void SetViewAngle(float Angle)
	{
		m_ViewAngle = Angle;

		if (m_CameraType == Camera_Type::Cam3D)
			CreateProjectionMatrix();
	}

	void SetDistance(float Distance)
	{
		m_Distance = Distance;

		CreateProjectionMatrix();
	}

	void SetCameraType(Camera_Type Type)
	{
		m_CameraType = Type;

		CreateProjectionMatrix();
	}

private:
	void CreateProjectionMatrix();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CCamera* Clone();
};

