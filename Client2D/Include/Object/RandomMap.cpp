#include "RandomMap.h"
#include "Scene/Scene.h"
#include "Resource/Material.h"

CRandomMap::CRandomMap()
{
}

CRandomMap::CRandomMap(const CRandomMap& obj) :
	CGameObject(obj)
{
	m_RandomMap = (CTileMapComponent*)FindSceneComponent("RandomMap");
}

CRandomMap::~CRandomMap()
{
}

void CRandomMap::Start()
{
	CGameObject::Start();
}

bool CRandomMap::Init()
{
	CGameObject::Init();

	m_RandomMap = CreateSceneComponent<CTileMapComponent>("RandomMap");

	SetRootComponent(m_RandomMap);
	m_RandomMap->CreateTile<CTile>(Tile_Shape::Rect,100,100, Vector2(80,80));
	m_RandomMap->SetMaterial(0, "Tile_Land");
	//m_RandomMap->GetMaterial(0)->SetShader();
	m_RandomMap->SetFrameMax(1, 1);
	m_RandomMap->SetTileDefaultFrame(1, 1);
	m_RandomMap->SetRelativePos(0.f, 0.f,0.f);
	m_RandomMap->SetAnimation2DEnable(false);

	/*m_TileMap->CreateTile<CTile>(Tile_Shape::Rhombus, 100, 100, Vector2(160.f, 80.f));

	m_TileMap->SetMaterial(0, "MainMap");
	m_TileMap->SetFrameMax(5, 76);
	m_TileMap->SetTileDefaultFrame(3, 2);*/

	//m_TileMap->SetRelativePos(100.f, 500.f, 0.f);

	/*m_TileMap->CreateTile<CTile>(Tile_Shape::Rect, 100, 100, Vector2(64.f, 64.f));

	m_TileMap->SetMaterial(0, "MainMapRect");
	m_TileMap->SetFrameMax(1, 5);
	m_TileMap->SetTileDefaultFrame(0, 0);*/

	//m_TileMap->Load(TEXT("MainMap.map"));
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
