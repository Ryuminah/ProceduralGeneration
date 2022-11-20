
#include "MainScene.h"
#include "Scene/Scene.h"
#include "../Object/Player2D.h"
#include "Scene/SceneResource.h"
#include "Render/RenderManager.h"
#include "../UI/MainHUDWidget.h"
#include "Scene/Viewport.h"
#include "../Object/MainMap.h"
#include "../MapGenerator/RandomMap.h"
#include "../MapGenerator/MapEnum.h"
#include "../Object/ImageObject.h"
#include "../GenerateWindow.h"
#include "IMGUIManager.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	CreateMaterial();
	CreateAnimationSequence2D();
	
	CImageObject* pImageObject = m_pScene->SpawnObject<CImageObject>("BackGround");
	pImageObject->SetRelativePos(-2000.f, -1000.f, 0.f);

	CRandomMap* RandomMap = m_pScene->SpawnObject<CRandomMap>("RandomMap");
	RandomMap->SetRelativePos(0.f, 0.f, 0.01f);

	CMapCamera* pCamera = m_pScene->SpawnObject<CMapCamera>("MapCamera");
	
	m_GenerateWindow = (CGenerateWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("GenerateWindow");
	m_GenerateWindow->Open();

	return true;
}

void CMainScene::CreateMaterial()
{
	// BackGround
	m_pScene->GetResource()->CreateMaterial("Background");
	m_pScene->GetResource()->AddMaterialTexture("Background", "Background",
		TEXT("Background.png"));
	m_pScene->GetResource()->SetMaterialTransparency("Background", true);
}

void CMainScene::CreateAnimationSequence2D()
{
	// Create Image
	m_pScene->GetResource()->CreateAnimationSequence2D("Image_Background");
	m_pScene->GetResource()->SetAnimationSequence2DTexture("Image_Background",
		"Image_Background", TEXT("RandomMap/Background.png"));
	m_pScene->GetResource()->AddAnimationSequence2DFrame("Image_Background",
		Vector2(0.f, 0.f), Vector2(7200.f, 5000.f));

	m_pScene->GetResource()->CreateAnimationSequence2D("Image_BackgroundWhite");
	m_pScene->GetResource()->SetAnimationSequence2DTexture("Image_BackgroundWhite",
		"Image_BackgroundWhite", TEXT("RandomMap/Background_White.png"));
	m_pScene->GetResource()->AddAnimationSequence2DFrame("Image_BackgroundWhite",
		Vector2(0.f, 0.f), Vector2(7200.f, 5000.f));
}


