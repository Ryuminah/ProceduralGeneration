#include <time.h>
#include <random>
#include <set>
#include <map>
#include "MapGenerator.h"
#include "TileFinder.h"
#include "MapGeneratorManager.h"
#include "RandomMap.h"
#include "Scene/Scene.h"
#include "Resource/Material.h"
#include "Scene/SceneResource.h"
#include "Engine.h"
#include "Timer.h"
#include "Input.h"

CRandomMap::CRandomMap() : m_MapSizeX(50), m_MapSizeY(50)
{
}

CRandomMap::CRandomMap(const CRandomMap& obj) :
	CGameObject(obj)
{
	m_MapComponent = (CTileMapComponent*)FindSceneComponent("RandomMap");
}

CRandomMap::~CRandomMap()
{
}

void CRandomMap::Start()
{
	CGameObject::Start();

	//GenerateMapBase();
}

bool CRandomMap::Init()
{
	CGameObject::Init();

	// Load RandomMap Texture 
	m_pScene->GetResource()->CreateMaterial("Tile_16px");
	m_pScene->GetResource()->AddMaterialTexture("Tile_16px", "Tile_16px",
		TEXT("RandomMap/Tile_16px.png"));
	m_pScene->GetResource()->SetMaterialTransparency("Tile_16px", true);
	m_pScene->GetResource()->SetMaterialShader("Tile_16px", "TileMapShader");

	m_pScene->GetResource()->CreateMaterial("Tile_Land");
	m_pScene->GetResource()->AddMaterialTexture("Tile_Land", "Tile_Land",
		TEXT("RandomMap/Tile_Land_16px.png"));
	m_pScene->GetResource()->SetMaterialTransparency("Tile_Land", true);
	m_pScene->GetResource()->SetMaterialShader("Tile_Land", "TileMapShader");
	
	m_pScene->GetResource()->CreateMaterial("Tile_Sea");
	m_pScene->GetResource()->AddMaterialTexture("Tile_Sea", "Tile_Sea",
		TEXT("RandomMap/Tile_Sea_16px.png"));
	m_pScene->GetResource()->SetMaterialTransparency("Tile_Sea", true);
	m_pScene->GetResource()->SetMaterialShader("Tile_Sea", "TileMapShader");
	
	m_MapGenerator = CMapGeneratorManager::GetInst()->CreateMapGenerator<CMapGenerator>("RandomMap", this);
	
	m_MapComponent->SetMaterial(0, "Tile_16px");
	m_MapComponent->SetFrameMax(6, 1);
	m_MapComponent->SetTileDefaultFrame(0, 0);
	m_MapComponent->SetPivot(0.f, 0.f, 0.f);
	m_MapComponent->SetRelativePos(0.f, 0.f, 0.1f);
	m_MapComponent->SetAnimation2DEnable(false);

	CInput::GetInst()->AddKeyCallback<CRandomMap>("Space", KT_Down, this, &CRandomMap::GenerateBase);
	CInput::GetInst()->AddKeyCallback<CRandomMap>("1", KT_Down, this, &CRandomMap::GenerateLand);
	CInput::GetInst()->AddKeyCallback<CRandomMap>("2", KT_Down, this, &CRandomMap::GenerateCoast);
	CInput::GetInst()->AddKeyCallback<CRandomMap>("3", KT_Down, this, &CRandomMap::GenerateForest);
	CInput::GetInst()->AddKeyCallback<CRandomMap>("Enter", KT_Down, this, &CRandomMap::Clear);


	return true;
}

void CRandomMap::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CRandomMap::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CRandomMap::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CRandomMap::Render(float DeltaTime)
{
	CGameObject::Render(DeltaTime);
}

CRandomMap* CRandomMap::Clone()
{
	return new CRandomMap(*this);
}

void CRandomMap::GenerateLand(float DeltaTime)
{
	m_MapGenerator->GenerateWorld(TILE_STATE::LAND);
}

void CRandomMap::GenerateCoast(float DeltaTime)
{
	m_MapGenerator->GenerateWorld(TILE_STATE::COAST);
}

void CRandomMap::GenerateForest(float DeltaTime)
{
	m_MapGenerator->GenerateWorld(TILE_STATE::FOREST);
}

void CRandomMap::GenerateBase(float DeltaTime)
{
	m_MapGenerator->GenerateWorld(TILE_STATE::BASE);
}

void CRandomMap::Clear(float DeltaTime)
{
	m_MapGenerator->GenerateWorld(TILE_STATE::CLEAR);
}


void CRandomMap::GenerateEnvironment(float DeltaTime)
{
	// 땅, 바다 조성 이후 환경 조성

	// 땅 가생이를 판단한다.
}
