
#include "Monster.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Material.h"

CMonster::CMonster() :
    m_Animation(nullptr)
{
}

CMonster::CMonster(const CMonster& obj) :
    CGameObject(obj)
{
    m_Root = (CAnimationMeshComponent*)FindSceneComponent("Root");
    m_Body = (CColliderCube*)FindSceneComponent("MonsterBody");
    m_Mesh = (CAnimationMeshComponent*)FindSceneComponent("Mesh");
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

    m_Root = CreateSceneComponent<CSceneComponent>("Root");
    m_Mesh = CreateSceneComponent<CAnimationMeshComponent>("Mesh");
    m_Body = CreateSceneComponent<CColliderCube>("MonsterBody");

    SetRootComponent(m_Root);

    m_Root->AddChild(m_Body);

    m_Body->AddChild(m_Mesh);

    m_Mesh->SetInheritRotY(true);

    m_Mesh->SetMesh("PlayerMesh");

    m_Mesh->SetWorldScale(0.01f, 0.01f, 0.01f);
    m_Mesh->SetWorldPos(0.f, 0.f, 0.f);
    m_Mesh->SetRelativeRotationY(180.f);

    m_Animation = new CAnimation;

    m_Mesh->SetAnimation(m_Animation);

    m_Animation->AddAnimationSequence("PlayerIdle");

    m_Body->SetVolume(3.f, 3.f, 3.f);
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
