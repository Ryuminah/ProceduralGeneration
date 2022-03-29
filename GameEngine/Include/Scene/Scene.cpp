
#include "Scene.h"
#include "../Component/StaticMeshComponent.h"

CScene::CScene()    :
    m_pSceneMode(nullptr),
    m_pSceneResource(nullptr),
    m_pCameraManager(nullptr),
    m_pCollision(nullptr),
    m_pViewport(nullptr),
    m_LightManager(nullptr),
    m_StartScene(false)
{
}

CScene::~CScene()
{
    m_ObjList.clear();

    SAFE_DELETE(m_LightManager);
    SAFE_DELETE(m_pViewport);
    SAFE_DELETE(m_pCollision);
    SAFE_DELETE(m_pCameraManager);
    SAFE_DELETE(m_pSceneMode);
    SAFE_DELETE(m_pSceneResource);
}

CGameObject* CScene::FindObject(const std::string& Name)
{
    auto    iter = m_ObjList.begin();
    auto    iterEnd = m_ObjList.end();

    for (; iter != iterEnd; ++iter)
    {
        if ((*iter)->GetName() == Name)
            return *iter;
    }

    return nullptr;
}

void CScene::Start()
{
    m_StartScene = true;

    auto    iter = m_ObjList.begin();
    auto    iterEnd = m_ObjList.end();

    for (; iter != iterEnd; ++iter)
    {
        (*iter)->Start();
    }

    m_pCameraManager->Start();

    m_pViewport->Start();

    m_pCollision->Start();

    m_Sky->Start();
}

bool CScene::Init()
{
    m_pSceneMode = new CSceneMode;

    m_pSceneMode->m_pScene = this;

    if (!m_pSceneMode->Init())
        return false;

    m_pSceneResource = new CSceneResource;

    m_pSceneResource->m_pScene = this;

    if (!m_pSceneResource->Init())
        return false;

    m_LightManager = new CLightManager;

    m_LightManager->m_pScene = this;

    if (!m_LightManager->Init())
        return false;

    m_pCameraManager = new CCameraManager;

    m_pCameraManager->m_pScene = this;

    if (!m_pCameraManager->Init())
        return false;

    m_pCollision = new CSceneCollision;

    m_pCollision->m_pScene = this;

    if (!m_pCollision->Init())
        return false;

    m_pViewport = new CViewport;

    m_pViewport->m_Scene = this;

    if (!m_pViewport->Init())
        return false;

    // 기본 Sky 생성
    m_Sky = new CGameObject;

    m_Sky->SetName("Sky");
    m_Sky->m_pScene = this;

    m_SkyMesh = m_Sky->CreateSceneComponent<CStaticMeshComponent>("SkyMesh");

    m_Sky->SetRootComponent(m_SkyMesh);

    m_SkyMesh->SetRelativeScale(100000.f, 100000.f, 100000.f);
    m_SkyMesh->SetMesh("SpherePos");

    m_SkyMesh->AddMaterial("DefaultSky");

    m_Sky->Release();
    m_SkyMesh->Release();

    return true;
}

void CScene::Update(float DeltaTime)
{
    auto    iter = m_ObjList.begin();
    auto    iterEnd = m_ObjList.end();

    for (; iter != iterEnd;)
    {
        if (!(*iter)->IsActive())
        {
            iter = m_ObjList.erase(iter);
            iterEnd = m_ObjList.end();
            continue;
        }

        if ((*iter)->IsEnable())
        {
            if (!(*iter)->IsStart())
                (*iter)->Start();

            (*iter)->Update(DeltaTime);
        }

        ++iter;
    }

    m_pCameraManager->Update(DeltaTime);

    m_pViewport->Update(DeltaTime);

    m_Sky->Update(DeltaTime);
}

void CScene::PostUpdate(float DeltaTime)
{
    auto    iter = m_ObjList.begin();
    auto    iterEnd = m_ObjList.end();

    for (; iter != iterEnd;)
    {
        if (!(*iter)->IsActive())
        {
            iter = m_ObjList.erase(iter);
            iterEnd = m_ObjList.end();
            continue;
        }

        if ((*iter)->IsEnable())
        {
            if (!(*iter)->IsStart())
                (*iter)->Start();

            (*iter)->PostUpdate(DeltaTime);
        }

        ++iter;
    }

    m_pCameraManager->PostUpdate(DeltaTime);

    m_pViewport->PostUpdate(DeltaTime);

    m_Sky->PostUpdate(DeltaTime);
}

void CScene::Collision(float DeltaTime)
{
    m_pCollision->Collision(DeltaTime);

    m_LightManager->Update(DeltaTime);

    /*auto    iter = m_ObjList.begin();
    auto    iterEnd = m_ObjList.end();

    for (; iter != iterEnd;)
    {
        if (!(*iter)->IsActive())
        {
            iter = m_ObjList.erase(iter);
            iterEnd = m_ObjList.end();
            continue;
        }

        if ((*iter)->IsEnable())
        {
            if (!(*iter)->IsStart())
                (*iter)->Start();

            (*iter)->Collision(DeltaTime);
        }

        ++iter;
    }*/
}

void CScene::Render(float DeltaTime)
{
    auto    iter = m_ObjList.begin();
    auto    iterEnd = m_ObjList.end();

    for (; iter != iterEnd;)
    {
        if (!(*iter)->IsActive())
        {
            iter = m_ObjList.erase(iter);
            iterEnd = m_ObjList.end();
            continue;
        }

        if ((*iter)->IsEnable())
        {
            if (!(*iter)->IsStart())
                (*iter)->Start();

            (*iter)->PrevRender(DeltaTime);
        }

        ++iter;
    }
}

void CScene::Clear()
{
    auto    iter = m_ObjList.begin();
    auto    iterEnd = m_ObjList.end();

    for (; iter != iterEnd; ++iter)
    {
        (*iter)->ClearTransformState();
    }
}
