
#include "Bullet.h"
#include "FlameEffect.h"
#include "Scene/Scene.h"

CBullet::CBullet()  :
    m_Distance(500.f)
{
    m_Collision = false;
}

CBullet::CBullet(const CBullet& obj) :
    CGameObject(obj)
{
    m_Body = (CColliderCircle*)FindSceneComponent("Body");
    m_Sprite = (CSpriteComponent*)FindSceneComponent("Sprite");
    m_Collision = false;
}

CBullet::~CBullet()
{
}

void CBullet::Start()
{
    CGameObject::Start();
}

bool CBullet::Init()
{
    CGameObject::Init();

    m_Body = CreateSceneComponent<CColliderCircle>("Body");
    m_Sprite = CreateSceneComponent<CSpriteComponent>("Sprite");

    SetRootComponent(m_Sprite);

    m_Sprite->SetRelativeScale(50.f, 50.f, 1.f);
    m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

    m_Body->SetRadius(sqrtf(50.f * 50.f));
    //m_Body->SetCollisionProfile("Monster");

    m_Sprite->AddChild(m_Body);

    m_Body->AddCollisionCallbackFunction<CBullet>(Collision_State::Begin, this,
        &CBullet::CollisionBegin);

    return true;
}

void CBullet::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    float   Dist = 500.f * DeltaTime;

    AddRelativePos(GetAxis(AXIS_Y) * Dist);

    m_Distance -= Dist;

    if (m_Distance <= 0.f)
        Active(false);

    if (m_Collision)
    {
    }
}

void CBullet::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

void CBullet::Collision(float DeltaTime)
{
    CGameObject::Collision(DeltaTime);
}

void CBullet::Render(float DeltaTime)
{
    CGameObject::Render(DeltaTime);
}

CBullet* CBullet::Clone()
{
    return new CBullet(*this);
}

void CBullet::CollisionBegin(const HitResult& result, CCollider* Collider)
{
    Active(false);

    m_Collision = true;

    // 이펙트를 생성한다.
    CFlameEffect* pParticle = m_pScene->SpawnObject<CFlameEffect>("FlameEffect",
        GetWorldPos());

    pParticle->SetRelativePos(result.DestObject->GetWorldPos());

    CParticleSystemComponent* ParticleCom = (CParticleSystemComponent*)pParticle->FindSceneComponent("Particle");

    //ParticleCom->SetMoveDir(-1.f, 0.f, 0.f);

    // 이펙트 사운드를 생성한다.
}

void CBullet::CollisionEnd(const HitResult& result, CCollider* Collider)
{
    m_Collision = false;
}
