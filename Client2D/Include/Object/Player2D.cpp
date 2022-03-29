
#include "Player2D.h"
#include "Input.h"
#include "Bullet.h"
#include "Scene/Scene.h"
#include "Resource/Material.h"
#include "../Animation2D/PlayerAnimation2D.h"
#include "Engine.h"
#include "../UI/PlayerWorldInfoWidget.h"

CPlayer2D::CPlayer2D()  :
	m_ChildFire(false),
	m_ChildFireTime(0.f),
	m_ChildFireTimeMax(0.2f),
	m_ChildFireDurationTime(0.f),
	m_ChildFireDurationTimeMax(5.f),
	m_PlayerInfoWidget(nullptr)
{
}

CPlayer2D::CPlayer2D(const CPlayer2D& obj)  :
	CGameObject(obj)
{
	m_ChildFire = false;

	m_Body = (CColliderBox2D*)FindSceneComponent("Body");
	m_Sprite = (CSpriteComponent*)FindSceneComponent("Sprite");
	m_Rotation = FindSceneComponent("Rotation");
	m_Muzzle = FindSceneComponent("Muzzle");
	m_SpriteLeftChild = (CSpriteComponent*)FindSceneComponent("LeftSprite");
	m_SpriteRightChild = (CSpriteComponent*)FindSceneComponent("RightSprite");
	m_LeftMuzzle = FindSceneComponent("LeftMuzzle");
	m_RightMuzzle = FindSceneComponent("RightMuzzle");
	m_Arm = (CSpringArm2D*)FindSceneComponent("Arm");
	m_Camera = (CCamera*)FindSceneComponent("Camera");
	m_PlayerInfoWidgetComponent = (CWidgetComponent*)FindSceneComponent("PlayerInfoWidget");
}

CPlayer2D::~CPlayer2D()
{
}

void CPlayer2D::Start()
{
	CGameObject::Start();
}

bool CPlayer2D::Init()
{
	CGameObject::Init();

	m_Body = CreateSceneComponent<CColliderBox2D>("Body");
	m_Sprite = CreateSceneComponent<CSpriteComponent>("Sprite");
	m_Rotation = CreateSceneComponent<CSceneComponent>("Rotation");
	m_Muzzle = CreateSceneComponent<CSceneComponent>("Muzzle");
	m_SpriteLeftChild = CreateSceneComponent<CSpriteComponent>("LeftSprite");
	m_SpriteRightChild = CreateSceneComponent<CSpriteComponent>("RightSprite");

	m_LeftMuzzle = CreateSceneComponent<CSceneComponent>("LeftMuzzle");
	m_RightMuzzle = CreateSceneComponent<CSceneComponent>("RightMuzzle");


	m_Arm = CreateSceneComponent<CSpringArm2D>("Arm");
	m_Camera = CreateSceneComponent<CCamera>("Camera");

	m_PlayerInfoWidgetComponent = CreateSceneComponent<CWidgetComponent>("PlayerInfoWidget");

	SetRootComponent(m_Sprite);

	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(200.f, 300.f, 0.f);
	//m_Sprite->SetRelativeRotationZ(30.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Sprite->GetMaterial(0)->SetOpacity(0.3f);

	m_PlayerInfoWidgetComponent->CreateWidget<CPlayerWorldInfoWidget>("PlayerInfo");

	m_PlayerInfoWidgetComponent->SetRelativePos(-100.f, 50.f, 0.f);
	m_PlayerInfoWidgetComponent->SetRelativeScale(200.f, 120.f, 1.f);


	m_Sprite->AddChild(m_PlayerInfoWidgetComponent);

	m_Body->SetExtent(50.f, 50.f);
	m_Body->SetCollisionProfile("Player");

	m_Sprite->AddChild(m_Body);

	m_Arm->SetOffset(-640.f, -360.f, 0.f);
	m_Arm->SetInheritPosZ(false);

	m_Sprite->AddChild(m_Arm);
	m_Arm->AddChild(m_Camera);

	CSharedPtr<CMaterial>   SpriteMtrl = m_Sprite->GetMaterial(0);

	//SpriteMtrl->SetBaseColor(1.f, 0.f, 0.f, 1.f);
	SpriteMtrl->AddTexture("PlayerTex", TEXT("teemo.png"));

	m_Sprite->CreateAnimation2D<CPlayerAnimation2D>();

	m_Animation2D = m_Sprite->GetAnimation2D();

	// 프레임 종료 콜백 설정
	m_Animation2D->SetFrameEndFunction<CPlayer2D>(this, &CPlayer2D::AnimationFrameEnd);

	m_Sprite->AddChild(m_Rotation);
	m_Sprite->AddChild(m_Muzzle);

	m_Muzzle->SetInheritRotZ(true);
	m_Muzzle->SetRelativePos(Vector3(0.f, 75.f, 0.f));
	m_Muzzle->SetPivot(0.5f, 0.5f, 0.f);

	m_Rotation->SetPivot(0.5f, 0.5f, 0.f);

	m_Rotation->AddChild(m_SpriteLeftChild);
	m_Rotation->AddChild(m_SpriteRightChild);

	m_SpriteLeftChild->SetRelativeScale(50.f, 50.f, 1.f);
	m_SpriteLeftChild->SetRelativePos(-100.f, 0.f, 0.f);
	m_SpriteLeftChild->SetRelativeRotationZ(30.f);
	m_SpriteLeftChild->SetPivot(0.5f, 0.5f, 0.f);
	m_SpriteLeftChild->SetInheritRotZ(true);

	m_SpriteLeftChild->SetMaterial(0, "PlayerChild");

	m_SpriteLeftChild->AddChild(m_LeftMuzzle);

	m_LeftMuzzle->SetInheritRotZ(true);
	m_LeftMuzzle->SetRelativePos(Vector3(0.f, 50.f, 0.f));
	m_LeftMuzzle->SetPivot(0.5f, 0.5f, 0.f);

	m_SpriteRightChild->SetRelativeScale(50.f, 50.f, 1.f);
	m_SpriteRightChild->SetRelativePos(100.f, 0.f, 0.f);
	//m_SpriteLeftChild->SetRelativeRotationZ(30.f);
	m_SpriteRightChild->SetPivot(0.5f, 0.5f, 0.f);
	m_SpriteRightChild->SetInheritRotZ(true);

	m_SpriteRightChild->SetMaterial(0, "PlayerChild");

	m_SpriteRightChild->AddChild(m_RightMuzzle);

	m_RightMuzzle->SetInheritRotZ(true);
	m_RightMuzzle->SetRelativePos(Vector3(0.f, 50.f, 0.f));
	m_RightMuzzle->SetPivot(0.5f, 0.5f, 0.f);

	CInput::GetInst()->AddKeyCallback<CPlayer2D>("MoveUp", KT_Push, this, &CPlayer2D::MoveUp);
	CInput::GetInst()->AddKeyCallback<CPlayer2D>("MoveDown", KT_Push, this, &CPlayer2D::MoveDown);
	CInput::GetInst()->AddKeyCallback<CPlayer2D>("RotationZ", KT_Push, this, &CPlayer2D::RotationZ);
	CInput::GetInst()->AddKeyCallback<CPlayer2D>("RotationZInv", KT_Push, this, &CPlayer2D::RotationZInv);
	CInput::GetInst()->AddKeyCallback<CPlayer2D>("Fire", KT_Down, this, &CPlayer2D::Fire);
	CInput::GetInst()->AddKeyCallback<CPlayer2D>("Skill1", KT_Down, this, &CPlayer2D::Triple);
	CInput::GetInst()->AddKeyCallback<CPlayer2D>("Skill2", KT_Down, this, &CPlayer2D::Skill2);
	CInput::GetInst()->AddKeyCallback<CPlayer2D>("MouseRButton", KT_Down, this, &CPlayer2D::MoveTarget);

	return true;
}

void CPlayer2D::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		//m_SpriteLeftChild->AddRelativePos(-30.f * DeltaTime, 0.f, 0.f);
		//m_SpriteRightChild->AddRelativePos(30.f * DeltaTime, 0.f, 0.f);
		m_Animation2D->SetSequencePlayRate("PlayerAttack", 5.f);
	}

	m_Rotation->AddRelativeRotationZ(90.f * DeltaTime);

	if (m_ChildFire)
	{
		m_ChildFireDurationTime += DeltaTime;

		if (m_ChildFireDurationTime >= m_ChildFireDurationTimeMax)
		{
			m_ChildFire = false;
			m_ChildFireDurationTime = 0.f;
			m_ChildFireTime = 0.f;
		}

		else
		{
			m_ChildFireTime += DeltaTime;

			if (m_ChildFireTime >= m_ChildFireTimeMax)
			{
				m_ChildFireTime -= m_ChildFireTimeMax;

				CBullet* pLeftBullet = m_pScene->SpawnObject<CBullet>("Bullet");

				pLeftBullet->SetRelativePos(m_LeftMuzzle->GetWorldPos());
				pLeftBullet->SetRelativeRotation(GetWorldRotation());

				CBullet* pRightBullet = m_pScene->SpawnObject<CBullet>("Bullet");

				pRightBullet->SetRelativePos(m_RightMuzzle->GetWorldPos());
				pRightBullet->SetRelativeRotation(GetWorldRotation());
			}
		}
	}
}

void CPlayer2D::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CPlayer2D::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CPlayer2D::Render(float DeltaTime)
{
	CGameObject::Render(DeltaTime);
}

CPlayer2D* CPlayer2D::Clone()
{
	return new CPlayer2D(*this);
}

void CPlayer2D::Animation2DNotify(const std::string& Name)
{
	if (Name == "Attack")
	{
		CBullet* pBullet = m_pScene->SpawnObject<CBullet>("Bullet");

		pBullet->SetRelativePos(m_Muzzle->GetWorldPos());
		pBullet->SetRelativeRotation(GetWorldRotation());
		pBullet->SetCollisionProfile("PlayerAttack");

		//CEngine::GetInst()->AddDebugLog("Player Attack");
	}
}

void CPlayer2D::MoveUp(float DeltaTime)
{
	AddRelativePos(GetAxis(AXIS_Y) * 300.f * DeltaTime);
}

void CPlayer2D::MoveDown(float DeltaTime)
{
	AddRelativePos(GetAxis(AXIS_Y) * -300.f * DeltaTime);
}

void CPlayer2D::RotationZ(float DeltaTime)
{
	AddRelativeRotationZ(360.f * DeltaTime);
}

void CPlayer2D::RotationZInv(float DeltaTime)
{
	AddRelativeRotationZ(-360.f * DeltaTime);
}

void CPlayer2D::Fire(float DeltaTime)
{
	m_Animation2D->ChangeAnimation("PlayerAttack");
}

void CPlayer2D::Triple(float DeltaTime)
{
	float   Angle = GetWorldRotation().z - 30.f;

	for (int i = 0; i < 3; ++i)
	{
		CBullet* pBullet = m_pScene->SpawnObject<CBullet>("Bullet");

		pBullet->SetRelativePos(m_Muzzle->GetWorldPos());
		pBullet->SetRelativeRotation(GetWorldRotation());
		pBullet->SetRelativeRotationZ(Angle);
		pBullet->SetCollisionProfile("PlayerAttack");

		Angle += 30.f;
	}
}

void CPlayer2D::Skill2(float DeltaTime)
{
	m_ChildFire = true;

	m_ChildFireTime = 0.f;
	m_ChildFireDurationTime = 0.f;
}

void CPlayer2D::MoveTarget(float DeltaTime)
{
	Vector2 MousePos = CInput::GetInst()->GetMouse2DWorldPos();

	Move(MousePos);
}

void CPlayer2D::AnimationFrameEnd(const std::string& Name)
{
	if (Name == "PlayerAttack")
		m_Animation2D->ChangeAnimation("PlayerIdle");
}
