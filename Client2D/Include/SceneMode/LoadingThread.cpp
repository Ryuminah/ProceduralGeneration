#include "LoadingThread.h"
#include "MainScene.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

CLoadingThread::CLoadingThread()
{
}

CLoadingThread::~CLoadingThread()
{
}

void CLoadingThread::Run()
{
	CScene* Scene = CSceneManager::GetInst()->CreateScene();

	Scene->SetSceneMode<CMainScene>();

	CSceneManager::GetInst()->SetNextScene(Scene);
}
