
#include "Particle2D.h"
#include "Scene/Scene.h"
#include "Resource/Material.h"

CParticle2D::CParticle2D()
{
}

CParticle2D::CParticle2D(const CParticle2D& obj) :
	CGameObject(obj)
{
	m_Particle = (CParticleSystemComponent*)FindSceneComponent("Particle");
}

CParticle2D::~CParticle2D()
{
}

void CParticle2D::Start()
{
	CGameObject::Start();
}

bool CParticle2D::Init()
{
	CGameObject::Init();

	m_Particle = CreateSceneComponent<CParticleSystemComponent>("Particle");
	
	SetRootComponent(m_Particle);

	m_Particle->SetRelativeScale(100.f, 100.f, 1.f);
	m_Particle->SetRelativePos(400.f, 100.f, 0.f);
	//m_Sprite->SetRelativeRotationZ(30.f);
	//m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	//CSharedPtr<CMaterial>   SpriteMtrl = m_Particle->GetMaterial(0);

	//SpriteMtrl->SetBaseColor(1.f, 0.f, 0.f, 1.f);
	//SpriteMtrl->AddTexture("PlayerTex", TEXT("teemo.png"));

	//m_Sprite->CreateAnimation2D<CPlayerAnimation2D>();

	//m_Animation2D = m_Sprite->GetAnimation2D();

	// 프레임 종료 콜백 설정
	//m_Animation2D->SetFrameEndFunction<CPlayer2D>(this, &CPlayer2D::AnimationFrameEnd);

	return true;
}

void CParticle2D::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CParticle2D::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CParticle2D::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CParticle2D::Render(float DeltaTime)
{
	CGameObject::Render(DeltaTime);
}

CParticle2D* CParticle2D::Clone()
{
	return new CParticle2D(*this);
}

void CParticle2D::Animation2DNotify(const std::string& Name)
{
}
