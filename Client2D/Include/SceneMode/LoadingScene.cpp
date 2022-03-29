#include "LoadingScene.h"
#include "../UI/LoadingUI.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "LoadingThread.h"
#include "ThreadManager.h"

CLoadingScene::CLoadingScene()
{
}

CLoadingScene::~CLoadingScene()
{
}

bool CLoadingScene::Init()
{
	CLoadingUI* Widget = m_pScene->GetViewport()->AddWindow<CLoadingUI>("Loading");

	// �ε��� �����带 �����Ѵ�.
	m_Thread = CThreadManager::GetInst()->CreateThread<CLoadingThread>("LoadingThread");

	m_Thread->SetLoop(false);
	m_Thread->Start();

	return true;
}
