
#include "MainLandScape.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Material.h"

CMainLandScape::CMainLandScape()
{
}

CMainLandScape::CMainLandScape(const CMainLandScape& obj) :
    CGameObject(obj)
{
    m_LandScape = (CLandScapeComponent*)FindSceneComponent("LandScape");
}

CMainLandScape::~CMainLandScape()
{
}

void CMainLandScape::Start()
{
    CGameObject::Start();
}

bool CMainLandScape::Init()
{
    CGameObject::Init();

    m_LandScape = CreateSceneComponent<CLandScapeComponent>("LandScape");

    SetRootComponent(m_LandScape);

    m_LandScape->CreateLandScape("MainLandScape", 129, 129, 1.f, 1.f, TEXT("LandScape/height1.bmp"));

    m_LandScape->AddMaterial("MainLandScape");
    m_LandScape->SetDetailLevel(60.f);
    m_LandScape->SetSplatCount(4);

    //CMaterial* Mtrl = m_pScene->GetResource()->FindMaterial("MainLandScape");

    return true;
}

void CMainLandScape::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CMainLandScape::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

void CMainLandScape::Collision(float DeltaTime)
{
    CGameObject::Collision(DeltaTime);
}

void CMainLandScape::Render(float DeltaTime)
{
    CGameObject::Render(DeltaTime);
}

CMainLandScape* CMainLandScape::Clone()
{
    return new CMainLandScape(*this);
}
