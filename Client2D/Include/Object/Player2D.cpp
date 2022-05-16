
#include "Player2D.h"
#include "Input.h"
#include "Bullet.h"
#include "Scene/Scene.h"
#include "Resource/Material.h"
#include "../Animation2D/PlayerAnimation2D.h"
#include "Engine.h"
#include "../UI/PlayerWorldInfoWidget.h"

CMapCamera::CMapCamera()  
{
}

CMapCamera::CMapCamera(const CMapCamera& obj)  :
	CGameObject(obj)
{
	m_Arm = (CSpringArm2D*)FindSceneComponent("Arm");
	m_Camera = (CCamera*)FindSceneComponent("Camera");
}

CMapCamera::~CMapCamera()
{
}

void CMapCamera::Start()
{
	CGameObject::Start();
}

bool CMapCamera::Init()
{
	CGameObject::Init();

	m_Arm = CreateSceneComponent<CSpringArm2D>("Arm");
	m_Camera = CreateSceneComponent<CCamera>("Camera");


	SetRootComponent(m_Arm);

	m_Arm->SetOffset(-640.f, -360.f, 0.f);
	m_Arm->SetInheritPosZ(false);
	m_Arm->AddChild(m_Camera);

	// 프레임 종료 콜백 설정
	CInput::GetInst()->AddKeyCallback<CMapCamera>("MoveUp", KT_Push, this, &CMapCamera::MoveUp);
	CInput::GetInst()->AddKeyCallback<CMapCamera>("MoveDown", KT_Push, this, &CMapCamera::MoveDown);
	CInput::GetInst()->AddKeyCallback<CMapCamera>("RotationZ", KT_Push, this, &CMapCamera::RotationZ);
	CInput::GetInst()->AddKeyCallback<CMapCamera>("RotationZInv", KT_Push, this, &CMapCamera::RotationZInv);


	return true;
}

void CMapCamera::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CMapCamera::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CMapCamera::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CMapCamera::Render(float DeltaTime)
{
	CGameObject::Render(DeltaTime);
}

CMapCamera* CMapCamera::Clone()
{
	return new CMapCamera(*this);
}

void CMapCamera::Animation2DNotify(const std::string& Name)
{
	//if (Name == "Attack")
	//{
	//	CBullet* pBullet = m_pScene->SpawnObject<CBullet>("Bullet");

	//	pBullet->SetRelativePos(m_Muzzle->GetWorldPos());
	//	pBullet->SetRelativeRotation(GetWorldRotation());
	//	pBullet->SetCollisionProfile("PlayerAttack");

	//	//CEngine::GetInst()->AddDebugLog("Player Attack");
	//}
}

void CMapCamera::MoveUp(float DeltaTime)
{
	AddRelativePos(GetAxis(AXIS_Y) * 300.f * DeltaTime);
}

void CMapCamera::MoveDown(float DeltaTime)
{
	AddRelativePos(GetAxis(AXIS_Y) * -300.f * DeltaTime);
}

void CMapCamera::RotationZ(float DeltaTime)
{
	AddRelativePos(GetAxis(AXIS_X) * -300.f * DeltaTime);
}

void CMapCamera::RotationZInv(float DeltaTime)
{
	AddRelativePos(GetAxis(AXIS_X) * 300.f * DeltaTime);
}

void CMapCamera::AnimationFrameEnd(const std::string& Name)
{

}
