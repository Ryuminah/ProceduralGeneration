
#include "Teemo.h"
#include "Bullet.h"
#include "Scene/Scene.h"
#include "Resource/Material.h"

CTeemo::CTeemo() :
	m_ChildFire(false),
	m_ChildFireTime(0.f),
	m_ChildFireTimeMax(0.2f),
	m_ChildFireDurationTime(0.f),
	m_ChildFireDurationTimeMax(5.f)
{
}

CTeemo::CTeemo(const CTeemo& obj) :
	CMonster(obj)
{
	m_ChildFire = false;

	m_Muzzle = FindSceneComponent("Muzzle");
}

CTeemo::~CTeemo()
{
}

void CTeemo::Start()
{
	CMonster::Start();
}

bool CTeemo::Init()
{
	CMonster::Init();

	m_Muzzle = CreateSceneComponent<CSceneComponent>("Muzzle");

	CSharedPtr<CMaterial>   SpriteMtrl = m_Sprite->GetMaterial(0);

	//SpriteMtrl->SetBaseColor(1.f, 0.f, 0.f, 1.f);
	SpriteMtrl->AddTexture("PlayerTex", TEXT("teemo.png"));

	//m_Sprite->CreateAnimation2D<CPlayerAnimation2D>();

	m_Animation2D = m_Sprite->GetAnimation2D();

	// 프레임 종료 콜백 설정
	//m_Animation2D->SetFrameEndFunction<CPlayer2D>(this, &CPlayer2D::AnimationFrameEnd);

	m_Sprite->AddChild(m_Muzzle);

	m_Muzzle->SetInheritRotZ(true);
	m_Muzzle->SetRelativePos(Vector3(0.f, 75.f, 0.f));
	m_Muzzle->SetPivot(0.5f, 0.5f, 0.f);

	return true;
}

void CTeemo::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CTeemo::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

void CTeemo::Collision(float DeltaTime)
{
	CMonster::Collision(DeltaTime);
}

void CTeemo::Render(float DeltaTime)
{
	CMonster::Render(DeltaTime);
}

CTeemo* CTeemo::Clone()
{
	return new CTeemo(*this);
}

void CTeemo::Animation2DNotify(const std::string& Name)
{
}

void CTeemo::AnimationFrameEnd(const std::string& Name)
{
}

