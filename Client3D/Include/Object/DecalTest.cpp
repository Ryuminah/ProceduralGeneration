
#include "DecalTest.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Material.h"

CDecalTest::CDecalTest()
{
}

CDecalTest::CDecalTest(const CDecalTest& obj) :
    CGameObject(obj)
{
    m_Decal = (CDecalComponent*)FindSceneComponent("Decal");
}

CDecalTest::~CDecalTest()
{
}

void CDecalTest::Start()
{
    CGameObject::Start();
}

bool CDecalTest::Init()
{
    CGameObject::Init();

    m_Decal = CreateSceneComponent<CDecalComponent>("Decal");

    SetRootComponent(m_Decal);

    CMaterial* Material = m_Decal->GetMaterial(0);

    //Material->SetTexture("DefaultDecal", "BloodDecal", TEXT("Decal/free-blood-texture_COLOR.png"));
    //Material->SetTexture("DefaultDecal", "BloodDecal", TEXT("HOS.jpg"));
    //Material->SetTexture("DefaultDecalNormal", "BloodDecalNormal", TEXT("Decal/free-blood-texture_NRM.png"));
    //Material->SetTexture("DefaultDecalSpecular", "BloodDecalSpecular", TEXT("Decal/free-blood-texture_SPEC.png"));

    Material->SetTexture("DefaultDecal", "BloodDecal", TEXT("Decal/Decal.png"));
    Material->SetTexture("DefaultDecalNormal", "BloodDecalNormal", TEXT("Decal/Decal_NRM.png"));
    Material->SetTexture("DefaultDecalSpecular", "BloodDecalSpecular", TEXT("Decal/Decal_SPEC.png"));

    m_Decal->SetFadeInTime(5.f);
    m_Decal->SetDuration(3.f);
    //m_Decal->SetFadeLoop(true);
    m_Decal->SetFadeOutTime(3.f);

    return true;
}

void CDecalTest::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    //m_Decal->AddRelativeRotationY(180.f * DeltaTime);
}

void CDecalTest::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

void CDecalTest::Collision(float DeltaTime)
{
    CGameObject::Collision(DeltaTime);
}

void CDecalTest::Render(float DeltaTime)
{
    CGameObject::Render(DeltaTime);
}

CDecalTest* CDecalTest::Clone()
{
    return new CDecalTest(*this);
}
