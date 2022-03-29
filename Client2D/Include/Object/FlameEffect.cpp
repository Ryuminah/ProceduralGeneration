
#include "FlameEffect.h"
#include "Scene/Scene.h"
#include "Resource/Material.h"

CFlameEffect::CFlameEffect()
{
}

CFlameEffect::CFlameEffect(const CFlameEffect& obj) :
	CGameObject(obj)
{
	m_Particle = (CParticleSystemComponent*)FindSceneComponent("Particle");
}

CFlameEffect::~CFlameEffect()
{
}

void CFlameEffect::Start()
{
	CGameObject::Start();
}

bool CFlameEffect::Init()
{
	CGameObject::Init();

	m_Particle = CreateSceneComponent<CParticleSystemComponent>("Particle");
	m_Distortion = CreateObjectComponent<CDistortionComponent>("Distortion");

	m_Particle->SetParticle("FlameParticle");

	//m_Particle->SetRelativePos(Vector3(150.f, 75.f, 0.f));
	m_Particle->SetPivot(0.5f, 0.5f, 0.f);
	m_Particle->SetSpawnTime(0.001f);
	m_Particle->SetGravityEnable(true);

	SetRootComponent(m_Particle);

	m_Distortion->SetTargetMaterial(m_Particle->GetMaterial(0));
	m_Distortion->EnableDistortion(true);
	m_Particle->SetDistortion(true);
	//m_Distortion->SetTargetPrimitive(m_Particle);

	SetLifeTime(9.9f);

	return true;
}

void CFlameEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CFlameEffect::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CFlameEffect::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CFlameEffect::Render(float DeltaTime)
{
	CGameObject::Render(DeltaTime);
}

CFlameEffect* CFlameEffect::Clone()
{
	return new CFlameEffect(*this);
}

void CFlameEffect::Animation2DNotify(const std::string& Name)
{
}
