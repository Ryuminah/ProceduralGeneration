#include "Engine.h"
#include "Device.h"
#include "Resource/ResourceManager.h"
#include "PathManager.h"
#include "Resource/ShaderManager.h"
#include "Resource/Shader.h"
#include "Resource/Mesh.h"
#include "Scene/SceneManager.h"
#include "Render/RenderManager.h"
#include "Timer.h"
#include "Input.h"
#include "CollisionManager.h"
#include "IMGUIManager.h"
#include "DebugLogWindow.h"
#include "Resource/GlobalCBuffer.h"
#include "Component/Camera.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "ThreadManager.h"
#include "NavigationManager.h"

DEFINITION_SINGLE(CEngine)

bool CEngine::m_Loop = true;

CEngine::CEngine()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(72584);

	m_pTimer = nullptr;
	m_TimeScale = 1.f;
	m_DebugLogWindow = nullptr;
	m_OnLogFPS = false;
	m_GlobalCBuffer = nullptr;
	m_Start = false;
}

CEngine::~CEngine()
{
	CSceneManager::DestroyInst();

	CThreadManager::DestroyInst();

	CNavigationManager::DestroyInst();

	CInput::DestroyInst();

	CCollisionManager::DestroyInst();

	CRenderManager::DestroyInst();

	CResourceManager::DestroyInst();

	CPathManager::DestroyInst();

	CIMGUIManager::DestroyInst();

	SAFE_DELETE(m_GlobalCBuffer);

	CDevice::DestroyInst();

	SAFE_DELETE(m_pTimer);
}

void CEngine::OnDebugLog()
{
	if (!m_DebugLogWindow)
	{
		m_DebugLogWindow = CIMGUIManager::GetInst()->AddWindow<CDebugLogWindow>("DebugLog");
	}
}

void CEngine::AddDebugLog(const char* Text)
{
	if (m_DebugLogWindow)
		m_DebugLogWindow->AddLog(Text);
}

void CEngine::Exit()
{
	DestroyWindow(m_hWnd);
}

bool CEngine::Init(const TCHAR* pClass, const TCHAR* pTitle, HINSTANCE hInst,
    int IconID, int iSmallIconID, int Width, int Height, bool ScreenMode)
{
    m_hInst = hInst;

	SetResolution(Width, Height);

	Register(pClass, IconID, iSmallIconID);
	Create(pClass, pTitle);

	return Init(hInst, m_hWnd, Width, Height, ScreenMode);
}

bool CEngine::Init(HINSTANCE hInst, HWND hWnd, int Width, int Height, 
	bool ScreenMode)
{
	m_hInst = hInst;
	m_hWnd = hWnd;

	SetResolution(Width, Height);

	// 디바이스를 초기화한다.
	if (!CDevice::GetInst()->Init(m_hWnd, Width, Height, ScreenMode))
		return false;

	// 경로관리자를 초기화한다.
	if (!CPathManager::GetInst()->Init())
		return false;

	// 렌더링 관리자 초기화
	if (!CRenderManager::GetInst()->Init())
		return false;

	// 리소스 관리자 초기화
	if (!CResourceManager::GetInst()->Init())
		return false;

	CRenderManager::GetInst()->SetDefaultTarget();

	// 충돌관리자 초기화
	if (!CCollisionManager::GetInst()->Init())
		return false;

	// 입력 관리자 초기화
	if (!CInput::GetInst()->Init(m_hInst, m_hWnd))
		return false;

	// 스레드 관리자 초기화
	if (!CThreadManager::GetInst()->Init())
		return false;

	// IMGUI 초기화
	if(!CIMGUIManager::GetInst()->Init(m_hWnd))
		return false;

	// Navigation 관리자 초기화
	if (!CNavigationManager::GetInst()->Init())
		return false;

	CNavigationManager::GetInst()->CreateNavigation();

	// 장면 관리자 초기화
	if (!CSceneManager::GetInst()->Init())
		return false;

	m_pTimer = new CTimer;

	m_pTimer->Init();

	m_GlobalCBuffer = new CGlobalCBuffer;

	m_GlobalCBuffer->Init();

	m_AccTime = 0.f;

	m_GlobalCBuffer->SetResolution(Vector2((float)m_RS.Width, (float)m_RS.Height));
	m_GlobalCBuffer->SetNoiseResolution(Vector2(340.f, 340.f));

	return true;
}

int CEngine::Run()
{
	MSG	msg = {};

	while (m_Loop)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CEngine::RunNoLoop()
{
	Logic();
}

void CEngine::Logic()
{
	m_pTimer->Update();

	CDevice::GetInst()->Update();

	CResourceManager::GetInst()->Update();

	if (m_DebugLogWindow && m_OnLogFPS && m_pTimer->IsFPS())
	{
		char	FPSLog[256] = {};
		sprintf_s(FPSLog, "FPS : %.5f", m_pTimer->GetFPS());
		m_DebugLogWindow->AddLog(FPSLog);
	}

	float	DeltaTime = m_pTimer->GetDeltaTime() * m_TimeScale;
	m_AccTime += DeltaTime;

	m_GlobalCBuffer->SetAccTime(m_AccTime);
	m_GlobalCBuffer->SetDeltaTime(DeltaTime);


	CInput::GetInst()->Update(DeltaTime);

	CIMGUIManager::GetInst()->Update(DeltaTime);

	CNavigationManager::GetInst()->Update(DeltaTime);

	if (Update(DeltaTime))
	{
		CIMGUIManager::GetInst()->Render();
		return;
	}

	CCamera* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	if (Camera)
		m_GlobalCBuffer->SetCameraBottom(Camera->GetCameraBottom());

	m_GlobalCBuffer->UpdateCBuffer();

	if (PostUpdate(DeltaTime))
	{
		CIMGUIManager::GetInst()->Render();
		return;
	}

	if (Collision(DeltaTime))
	{
		CIMGUIManager::GetInst()->Render();
		return;
	}

	if (Render(DeltaTime))
		return;
}

bool CEngine::Update(float DeltaTime)
{
	if (m_MouseWidget)
		m_MouseWidget->Update(DeltaTime);

	return CSceneManager::GetInst()->Update(DeltaTime);
}

bool CEngine::PostUpdate(float DeltaTime)
{
	if (m_MouseWidget)
		m_MouseWidget->PostUpdate(DeltaTime);

	return CSceneManager::GetInst()->PostUpdate(DeltaTime);
}

bool CEngine::Collision(float DeltaTime)
{
	return CSceneManager::GetInst()->Collision(DeltaTime);
}

bool CEngine::Render(float DeltaTime)
{
	CDevice::GetInst()->ClearTarget();

	// 모든 물체들을 출력한다.
	CSceneManager::GetInst()->Render(DeltaTime);

	CRenderManager::GetInst()->Render(DeltaTime);

	CIMGUIManager::GetInst()->Render();

	CDevice::GetInst()->Render();

	CSceneManager::GetInst()->Clear();

	return false;
}

void CEngine::Register(const TCHAR* pClass, int IconID, int SmallIconID)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IconID));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;// MAKEINTRESOURCEW(IDC_MY191101);
	wcex.lpszClassName = pClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(SmallIconID));

	RegisterClassExW(&wcex);
}

bool CEngine::Create(const TCHAR* pClass, const TCHAR* pTitle)
{
	m_hWnd = CreateWindow(pClass, pTitle, WS_OVERLAPPEDWINDOW, 0, 0, m_RS.Width, m_RS.Height,
		nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
		return false;

	RECT	rc = { 0, 0, m_RS.Width, m_RS.Height };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOZORDER | SWP_NOMOVE);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CEngine::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_DESTROY:
		m_Loop = false;
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

void CEngine::SetMouseWidget()
{
	CRenderManager::GetInst()->SetMouseWidget(m_MouseWidget);

	ShowCursor(FALSE);
}
