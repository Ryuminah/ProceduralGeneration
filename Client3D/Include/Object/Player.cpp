
#include "Player.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Material.h"
#include "Input.h"

CPlayer::CPlayer()  :
    m_Animation(nullptr)
{
}

CPlayer::CPlayer(const CPlayer& obj) :
    CGameObject(obj)
{
    m_Mesh = (CAnimationMeshComponent*)FindSceneComponent("Mesh");
    m_Body = (CColliderCube*)FindSceneComponent("Body");
    m_Mesh = (CAnimationMeshComponent*)FindSceneComponent("Mesh");
    m_Camera = (CCamera*)FindSceneComponent("Camera");
    m_Arm = (CSpringArm3D*)FindSceneComponent("Arm");
}

CPlayer::~CPlayer()
{
}

void CPlayer::Start()
{
    CGameObject::Start();
}

bool CPlayer::Init()
{
    CGameObject::Init();

    m_Root = CreateSceneComponent<CSceneComponent>("Root");
    m_Mesh = CreateSceneComponent<CAnimationMeshComponent>("Mesh");
    m_Camera = CreateSceneComponent<CCamera>("Camera");
    m_Arm = CreateSceneComponent<CSpringArm3D>("Arm");
    m_Body = CreateSceneComponent<CColliderCube>("Body");

    SetRootComponent(m_Root);

    m_Root->AddChild(m_Body);

    m_Body->AddChild(m_Mesh);

    m_Mesh->AddChild(m_Arm);

    m_Arm->AddChild(m_Camera);

    m_Body->SetInheritRotY(true);
    m_Mesh->SetInheritRotY(true);

    m_Camera->SetInheritRotX(true);
    m_Camera->SetInheritRotY(true);
    m_Camera->SetInheritRotZ(true);
    m_Arm->SetInheritRotY(false);

    m_Arm->SetRelativeRotationX(25.f);
    m_Arm->SetLength(3.f);
    m_Arm->SetOffset(0.f, 1.f, 0.f);

    m_Mesh->SetMesh("PlayerMesh");

    m_Mesh->SetWorldScale(0.01f, 0.01f, 0.01f);
    m_Mesh->SetWorldPos(0.f, 0.f, 0.f);
    m_Mesh->SetRelativeRotationY(180.f);

    m_Animation = new CAnimation;

    m_Mesh->SetAnimation(m_Animation);

    m_Animation->AddAnimationSequence("PlayerIdle");

    CInput::GetInst()->AddKeyCallback("MoveFront", Key_Type::KT_Push, this, &CPlayer::MoveFront);
    CInput::GetInst()->AddKeyCallback("MoveBack", Key_Type::KT_Push, this, &CPlayer::MoveBack);
    CInput::GetInst()->AddKeyCallback("RotationY", Key_Type::KT_Push, this, &CPlayer::RotationY);
    CInput::GetInst()->AddKeyCallback("RotationYInv", Key_Type::KT_Push, this, &CPlayer::RotationYInv);

    for (int i = 0; i < m_Mesh->GetMaterialCount(); ++i)
    {
        m_Mesh->GetMaterial(i)->SetReceiveDecal(false);
    }

    return true;
}

void CPlayer::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CPlayer::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

void CPlayer::Collision(float DeltaTime)
{
    CGameObject::Collision(DeltaTime);
}

void CPlayer::Render(float DeltaTime)
{
    CGameObject::Render(DeltaTime);
}

CPlayer* CPlayer::Clone()
{
    return new CPlayer(*this);
}

void CPlayer::MoveFront(float Time)
{
    AddRelativePos(GetAxis(AXIS_Z) * 3.f * Time);
}

void CPlayer::MoveBack(float Time)
{
    AddRelativePos(GetAxis(AXIS_Z) * -3.f * Time);
}

void CPlayer::RotationY(float Time)
{
    AddRelativeRotationY(-180.f * Time);
}

void CPlayer::RotationYInv(float Time)
{
    AddRelativeRotationY(180.f * Time);
}
