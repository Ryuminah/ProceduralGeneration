
#include "Camera.h"
#include "../Render/RenderManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Device.h"
#include "../Scene/LightManager.h"
#include "LightComponent.h"

CCamera::CCamera()
{
	m_SceneComponentType = SceneComponent_Type::Camera;
	m_CameraType = Camera_Type::Cam3D;

	m_ViewAngle = 90.f;
	m_Distance = 1000.f;
	m_CameraLeft = 0.f;
	m_CameraBottom = 0.f;

	SetTransformState(Transform_State::None);
}

CCamera::CCamera(const CCamera& com) :
	CSceneComponent(com)
{
	m_CameraType = com.m_CameraType;
	m_ViewAngle = com.m_ViewAngle;
	m_Distance = com.m_Distance;
	m_matView = com.m_matView;
	m_matProj = com.m_matProj;
	m_CameraLeft = com.m_CameraLeft;
	m_CameraBottom = com.m_CameraBottom;
	m_ShadowTargetLight = com.m_ShadowTargetLight;
}

CCamera::~CCamera()
{
}

void CCamera::SetShadowTargetLight(CLightComponent* Light)
{
	m_ShadowTargetLight = Light;
}

void CCamera::CreateProjectionMatrix()
{
	Resolution  RS = CDevice::GetInst()->GetResolution();

	switch (m_CameraType)
	{
	case Camera_Type::Cam3D:
	{
		float	Angle = XMConvertToRadians(m_ViewAngle);

		m_matProj = XMMatrixPerspectiveFovLH(Angle,
			RS.Width / (float)RS.Height, 0.1f, m_Distance);

		// 그림자 맵 용 투영행렬
		m_matShadowProj = XMMatrixPerspectiveFovLH(PI / 2.f, 1.f, 0.1f, m_Distance);
	}
		break;
	case Camera_Type::Cam2D:
	case Camera_Type::CamUI:
	{
		m_matProj = XMMatrixOrthographicOffCenterLH(0.f, (float)RS.Width, 0.f, (float)RS.Height,
			0.f, m_Distance);
	}
		break;
	}
}

void CCamera::Start()
{
	CSceneComponent::Start();

	CreateProjectionMatrix();

	m_RS = CDevice::GetInst()->GetResolution();
}

bool CCamera::Init()
{
	if (!CSceneComponent::Init())
		return false;

	// 기본으로 글로벌라이트를 지정해둔다.
	m_ShadowTargetLight = m_pScene->GetLightManager()->GetGlobalLightComponent();

	return true;
}

void CCamera::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	Resolution  RS = CDevice::GetInst()->GetResolution();

	m_CameraBottom = GetWorldPos().y - RS.Height / 2.f;
	m_CameraLeft = GetWorldPos().x - RS.Width / 2.f;
}

void CCamera::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	m_matView.Identity();

	for (int i = 0; i < AXIS_END; ++i)
	{
		Vector3	Axis = GetAxis((AXIS)i);
		memcpy(&m_matView[i][0], &Axis, sizeof(Vector3));
	}

	// 회전부분을 전치해서 최종 View의 회전으로 만들어준다.
	m_matView.Transpose();

	for (int i = 0; i < AXIS_END; ++i)
	{
		Vector3	Axis = GetAxis((AXIS)i);
		Vector3	Pos = GetWorldPos() * -1.f;
		m_matView[3][i] = Axis.Dot(Pos);
	}

	if (m_ShadowTargetLight)
	{
		m_matShadowView.Identity();

		Vector3	LightPos = (GetWorldPos() + m_ShadowTargetLight->GetWorldPos() + m_ShadowTargetLight->GetAxis(AXIS_Z) * -10.f);

		Vector3	Axis[AXIS_END] = {};

		Axis[AXIS_Y] = Vector3(0.f, 1.f, 0.f);

		Axis[AXIS_X] = Axis[AXIS_Y].Cross(m_ShadowTargetLight->GetAxis(AXIS_Z));
		Axis[AXIS_X].Normalize();

		Axis[AXIS_Y] = m_ShadowTargetLight->GetAxis(AXIS_Z).Cross(Axis[AXIS_X]);
		Axis[AXIS_Y].Normalize();

		Axis[AXIS_Z] = m_ShadowTargetLight->GetAxis(AXIS_Z);

		for (int i = 0; i < AXIS_END; ++i)
		{
			memcpy(&m_matShadowView[i][0], &Axis[i], sizeof(Vector3));
		}

		// 회전부분을 전치해서 최종 View의 회전으로 만들어준다.
		m_matShadowView.Transpose();

		for (int i = 0; i < AXIS_END; ++i)
		{
			m_matShadowView[3][i] = Axis[i].Dot(LightPos * -1.f);
		}
	}

	/*
	1 0 0 0
	0 1 0 0 
	0 0 1 0
	-x -y -z 1

	Xx Yx Zx 0
	Xy Yy Zy 0
	Xz Yz Zz 0
	0  0  0  1

	Xx Yx Zx 0
	Xy Yy Zy 0
	Xz Yz Zz 0
	-P.X -P.Y -P.Z 1


	WorldCameraAxisX * ViewMatrix = 1, 0, 0
	WorldCameraAxisY * ViewMatrix = 0, 1, 0
	WorldCAmeraAxisZ * ViewMatrix = 0, 0, 1

	Xx Xy Xz   Xx Yx Zx    1 0 0
	Yx Yy Yz * Xy Yy Zy = 0 1 0
	Zx Zy Zz   Xz Yz Zz    0 0 1

	서로 정직교 하는 행렬을 직교행렬이라고 한다.
	직교행렬은 전치행렬과 역행렬이 같다.
	*/
}

void CCamera::Collision(float DeltaTime)
{
	CSceneComponent::Collision(DeltaTime);
}

void CCamera::PrevRender(float DeltaTime)
{
	CSceneComponent::PrevRender(DeltaTime);
}

void CCamera::Render(float DeltaTime)
{
	CSceneComponent::Render(DeltaTime);
}

CCamera* CCamera::Clone()
{
	return new CCamera(*this);
}
