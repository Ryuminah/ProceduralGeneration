
#include "MainScene.h"
#include "Scene/Scene.h"
#include "../Object/Player2D.h"
#include "../Object/Teemo.h"
#include "../Object/PixelCollisionTest.h"
#include "../Object/TestParticle.h"
#include "Scene/SceneResource.h"
#include "Render/RenderManager.h"
#include "../UI/MainHUDWidget.h"
#include "Scene/Viewport.h"
#include "../Object/MainMap.h"
#include "../MapGenerator/RandomMap.h"
#include "../MapGenerator/MapEnum.h"
#include "../Object/ImageObject.h"


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
	CreateParticle();

	//m_pScene->GetResource()->LoadSound("UI", false, "ButtonMouseOn",
	//	"TeemoSmile.mp3");
	//m_pScene->GetResource()->LoadSound("UI", false, "ButtonClick",
	//	"TeemoStartClicck.mp3");

	/*CPixelCollisionTest* pPixelCollisionTest = m_pScene->SpawnObject<CPixelCollisionTest>("PixelCollisionTest");
	pPixelCollisionTest->SetRelativePos(100.f, 200.f, 0.f);*/

	CImageObject* pImageObject = m_pScene->SpawnObject<CImageObject>("BackGround");
	pImageObject->SetRelativePos(-2000.f, -1000.f, 0.f);

	CRandomMap* RandomMap = m_pScene->SpawnObject<CRandomMap>("RandomMap");
	RandomMap->SetRelativePos(0.f, 0.f, 0.01f);

	CMapCamera* pPlayer = m_pScene->SpawnObject<CMapCamera>("MapCamera");
	//CMainMap* MainMap = m_pScene->SpawnObject<CMainMap>("MainMap");

	//CTestParticle* pParticle = m_pScene->SpawnObject<CTestParticle>("PixelCollisionTest");

	/*CMainHUDWidget* Widget = m_pScene->GetViewport()->AddWindow<CMainHUDWidget>("MainHUD");*/

	return true;
}

void CMainScene::CreateMaterial()
{
	m_pScene->GetResource()->CreateMaterial("PlayerChild");
	m_pScene->GetResource()->AddMaterialTexture("PlayerChild", "PlayerChild",
		TEXT("Teemo.jpg"));

	m_pScene->GetResource()->CreateMaterial("Snow");
	m_pScene->GetResource()->AddMaterialTexture("Snow", "Snow",
		TEXT("Particle/Bubbles50px.png"));
	m_pScene->GetResource()->SetMaterialTransparency("Snow", true);
	//m_pScene->GetResource()->SetMaterialOpacity("DefaultMaterial", 0.3f);

	m_pScene->GetResource()->CreateMaterial("Flame");
	m_pScene->GetResource()->AddMaterialTexture("Flame", "Flame",
		TEXT("Particle/particle_00.png"));
	m_pScene->GetResource()->SetMaterialTransparency("Flame", true);

	m_pScene->GetResource()->CreateMaterial("MainMap");
	m_pScene->GetResource()->AddMaterialTexture("MainMap", "MainMap",
		TEXT("Diablos_Lair_Floor_TRS/Diablos_Lair_Floor.png"));
	m_pScene->GetResource()->SetMaterialTransparency("MainMap", true);
	m_pScene->GetResource()->SetMaterialShader("MainMap", "TileMapShader");

	m_pScene->GetResource()->CreateMaterial("MainMapRect");
	m_pScene->GetResource()->AddMaterialTexture("MainMapRect", "MainMapRect",
		TEXT("Floors.png"));
	m_pScene->GetResource()->SetMaterialTransparency("MainMapRect", true);
	m_pScene->GetResource()->SetMaterialShader("MainMapRect", "TileMapShader");

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

	m_pScene->GetResource()->CreateAnimationSequence2D("PlayerIdle");
	m_pScene->GetResource()->SetAnimationSequence2DTexture("PlayerIdle",
		"PlayerAtlas", TEXT("Monster.png"));

	for (int i = 0; i < 14; ++i)
	{
		m_pScene->GetResource()->AddAnimationSequence2DFrame("PlayerIdle",
			Vector2(i * 45.f, 60.f), Vector2((i + 1) * 45.f, 120.f));
	}


	m_pScene->GetResource()->CreateAnimationSequence2D("PlayerRun");
	m_pScene->GetResource()->SetAnimationSequence2DTexture("PlayerRun",
		"PlayerAtlas", TEXT("Monster.png"));

	for (int i = 0; i < 8; ++i)
	{
		m_pScene->GetResource()->AddAnimationSequence2DFrame("PlayerRun",
			Vector2(i * 45.f, 300.f), Vector2((i + 1) * 45.f, 360.f));
	}


	m_pScene->GetResource()->CreateAnimationSequence2D("PlayerAttack");
	m_pScene->GetResource()->SetAnimationSequence2DTexture("PlayerAttack",
		"PlayerAtlas", TEXT("Monster.png"));

	for (int i = 0; i < 21; ++i)
	{
		m_pScene->GetResource()->AddAnimationSequence2DFrame("PlayerAttack",
			Vector2(i * 45.f, 180.f), Vector2((i + 1) * 45.f, 240.f));
	}

	m_pScene->GetResource()->AddAnimationSequence2DNotify("PlayerAttack", "Attack", 8);
}

void CMainScene::CreateParticle()
{
	CMaterial* SnowMaterial = m_pScene->GetResource()->FindMaterial("Snow");

	m_pScene->GetResource()->CreateParticle("TestParticle");
	m_pScene->GetResource()->SetParticleMaterial("TestParticle", SnowMaterial);
	m_pScene->GetResource()->SetParticleMaxParticleCount("TestParticle", 1000);
	m_pScene->GetResource()->SetParticleStartColor("TestParticle", 0.68627f, 0.8745f, 0.89411f, 0.8f);
	m_pScene->GetResource()->SetParticleEndColor("TestParticle", 0.68627f, 0.8745f, 0.89411f, 0.f);
	m_pScene->GetResource()->SetParticleStartScale("TestParticle", 100.f, 100.f, 1.f);
	m_pScene->GetResource()->SetParticleEndScale("TestParticle", 20.f, 20.f, 1.f);
	m_pScene->GetResource()->SetParticleLifeTimeMin("TestParticle", 2.f);
	m_pScene->GetResource()->SetParticleLifeTimeMax("TestParticle", 4.f);
	m_pScene->GetResource()->SetParticleRange("TestParticle", 100.f, 0.f, 0.f);
	m_pScene->GetResource()->SetParticleMinSpeed("TestParticle", 100.f);
	m_pScene->GetResource()->SetParticleMaxSpeed("TestParticle", 200.f);
	m_pScene->GetResource()->SetParticleMoveEnable("TestParticle", true);
	m_pScene->GetResource()->SetParticle2D("TestParticle", true);
	m_pScene->GetResource()->SetParticleMoveDir("TestParticle", 0.f, 1.f, 0.f);
	m_pScene->GetResource()->SetParticleMoveAngle("TestParticle", 0.f, 0.f, 90.f);

	CMaterial* FlameMaterial = m_pScene->GetResource()->FindMaterial("Flame");

	m_pScene->GetResource()->CreateParticle("FlameParticle");
	m_pScene->GetResource()->SetParticleMaterial("FlameParticle", FlameMaterial);
	m_pScene->GetResource()->SetParticleMaxParticleCount("FlameParticle", 500);
	m_pScene->GetResource()->SetParticleStartColor("FlameParticle", 0.94901f, 0.490019f, 0.04705f, 0.8f);
	m_pScene->GetResource()->SetParticleEndColor("FlameParticle", 0.94901f, 0.490019f, 0.04705f, 0.f);
	m_pScene->GetResource()->SetParticleStartScale("FlameParticle", 20.f, 20.f, 1.f);
	m_pScene->GetResource()->SetParticleEndScale("FlameParticle", 10.f, 10.f, 1.f);
	//m_pScene->GetResource()->SetParticleLifeTimeMin("FlameParticle", 0.5f);
	//m_pScene->GetResource()->SetParticleLifeTimeMax("FlameParticle", 0.8f);
	m_pScene->GetResource()->SetParticleLifeTimeMin("FlameParticle", 10.5f);
	m_pScene->GetResource()->SetParticleLifeTimeMax("FlameParticle", 10.8f);
	m_pScene->GetResource()->SetParticleRange("FlameParticle", 0.f, 0.f, 0.f);
	m_pScene->GetResource()->SetParticleMinSpeed("FlameParticle", 600.f);
	m_pScene->GetResource()->SetParticleMaxSpeed("FlameParticle", 800.f);
	m_pScene->GetResource()->SetParticleMoveEnable("FlameParticle", true);
	m_pScene->GetResource()->SetParticle2D("FlameParticle", true);
	m_pScene->GetResource()->SetParticleMoveDir("FlameParticle", 0.f, 1.f, 0.f);
	m_pScene->GetResource()->SetParticleMoveAngle("FlameParticle", 0.f, 0.f, 90.f);

}
