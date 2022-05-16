
#include "Monster.h"
#include "Bullet.h"
#include "Scene/Scene.h"
#include "Resource/Material.h"

CMonster::CMonster()
{
}

CMonster::CMonster(const CMonster& obj)	:
	CGameObject(obj)
{
	m_Body = (CColliderCircle*)FindSceneComponent("Body");
	m_Sprite = (CSpriteComponent*)FindSceneComponent("Sprite");
	m_PaperBurn = (CPaperBurnComponent*)FindObjectComponent("PaperBurn");
}

CMonster::~CMonster()
{
}

void CMonster::Start()
{
	CGameObject::Start();
}

bool CMonster::Init()
{
	CGameObject::Init();

	m_Body = CreateSceneComponent<CColliderCircle>("Body");
	m_Sprite = CreateSceneComponent<CSpriteComponent>("Sprite");
	m_PaperBurn = CreateObjectComponent<CPaperBurnComponent>("PaperBurn");

	m_PaperBurn->SetTargetMaterial(m_Sprite->GetMaterial(0));
	m_PaperBurn->SetBurnTime(3.f);
	/*m_PaperBurn->SetOutLineColorUNorm(0.2f, 0.2f, 0.2f);
	m_PaperBurn->SetCenterLineColor(255, 69, 0);
	m_PaperBurn->SetInLineColorUNorm(0.7f, 0.7f, 0.7f);*/

	m_PaperBurn->SetEndFunction<CMonster>(this, &CMonster::PaperBurnEndFunction);

	SetRootComponent(m_Sprite);

	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(200.f, 300.f, 0.f);
	//m_Sprite->SetRelativeRotationZ(30.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	m_Body->SetRadius(sqrtf(50.f * 50.f));
	m_Body->SetCollisionProfile("Monster");

	m_Sprite->AddChild(m_Body);

	CSharedPtr<CMaterial>   SpriteMtrl = m_Sprite->GetMaterial(0);

	//SpriteMtrl->SetBaseColor(1.f, 0.f, 0.f, 1.f);
	SpriteMtrl->AddTexture("PlayerTex", TEXT("teemo.png"));

	//m_Sprite->CreateAnimation2D<CPlayerAnimation2D>();

	m_Animation2D = m_Sprite->GetAnimation2D();

	// 프레임 종료 콜백 설정
	//m_Animation2D->SetFrameEndFunction<CMapCamera>(this, &CMapCamera::AnimationFrameEnd);

	m_Body->AddCollisionCallbackFunction<CMonster>(Collision_State::Begin, this,
		&CMonster::CollisionBegin);

	return true;
}

void CMonster::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CMonster::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CMonster::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CMonster::Render(float DeltaTime)
{
	CGameObject::Render(DeltaTime);
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

void CMonster::Animation2DNotify(const std::string& Name)
{
}

void CMonster::AnimationFrameEnd(const std::string& Name)
{
}

void CMonster::CollisionBegin(const HitResult& result, CCollider* Collider)
{
	m_PaperBurn->StartPaperBurn();
}

void CMonster::PaperBurnEndFunction()
{
	Active(false);
}
