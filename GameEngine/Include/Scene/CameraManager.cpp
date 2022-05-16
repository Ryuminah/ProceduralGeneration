
#include "CameraManager.h"

CCameraManager::CCameraManager()
{
}

CCameraManager::~CCameraManager()
{
}

void CCameraManager::SetCurrentCamera(const std::string& Name)
{
    CCamera* Camera = FindCamera(Name);

    if (!Camera)
        return;

    m_CurrentCamera = Camera;
}

void CCameraManager::AddCamera(CCamera* Camera)
{
    // 처음 등록이라면 이 카메라를 Current Camera로 만들어준다.
    if (m_CameraList.size() == 2)
        m_CurrentCamera = Camera;

    m_CameraList.push_back(Camera);
}

CCamera* CCameraManager::FindCamera(const std::string& Name)
{
    auto    iter = m_CameraList.begin();
    auto    iterEnd = m_CameraList.end();

    for (; iter != iterEnd; ++iter)
    {
        if ((*iter)->GetName() == Name)
            return *iter;
    }

    return nullptr;
}

void CCameraManager::Start()
{
    CCamera* Camera = FindCamera("DefaultCamera");
    Camera->Start();

    Camera = FindCamera("UICamera");
    Camera->Start();
}

bool CCameraManager::Init()
{
	// Default Camera 생성
	CCamera* Camera = new CCamera;

    Camera->SetName("DefaultCamera");
    Camera->m_pScene = m_pScene;
    //Camera->m_pOwner = this;
    Camera->SetCameraType(Camera_Type::Cam2D);

    if (!Camera->Init())
    {
        SAFE_RELEASE(Camera);
        return false;
    }

    m_CameraList.push_back(Camera);
    Camera->Release();

    m_CurrentCamera = Camera;

    // UI용 카메라를 생성한다.
    Camera = new CCamera;

    Camera->SetName("UICamera");
    Camera->m_pScene = m_pScene;
    //Camera->m_pOwner = this;
    Camera->SetCameraType(Camera_Type::CamUI);

    if (!Camera->Init())
    {
        SAFE_RELEASE(Camera);
        return false;
    }

    m_CameraList.push_back(Camera);
    Camera->Release();

    m_UICamera = Camera;

	return true;
}

void CCameraManager::Update(float DeltaTime)
{
    if (m_CurrentCamera->GetName() == "DefaultCamera")
        m_CurrentCamera->Update(DeltaTime);
}

void CCameraManager::PostUpdate(float DeltaTime)
{
    if (m_CurrentCamera->GetName() == "DefaultCamera")
        m_CurrentCamera->PostUpdate(DeltaTime);
}
