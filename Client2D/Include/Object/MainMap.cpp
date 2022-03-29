
#include "MainMap.h"
#include "Scene/Scene.h"
#include "Resource/Material.h"

CMainMap::CMainMap()
{
}

CMainMap::CMainMap(const CMainMap& obj) :
	CGameObject(obj)
{
	m_TileMap = (CTileMapComponent*)FindSceneComponent("TileMap");
}

CMainMap::~CMainMap()
{
}

void CMainMap::Start()
{
	CGameObject::Start();
}

bool CMainMap::Init()
{
	CGameObject::Init();

	m_TileMap = CreateSceneComponent<CTileMapComponent>("TileMap");

	SetRootComponent(m_TileMap);

	m_TileMap->Load(TEXT("MainMap.map"));

	/*m_TileMap->CreateTile<CTile>(Tile_Shape::Rhombus, 100, 100, Vector2(160.f, 80.f));

	m_TileMap->SetMaterial(0, "MainMap");
	m_TileMap->SetFrameMax(5, 76);
	m_TileMap->SetTileDefaultFrame(3, 2);*/

	//m_TileMap->SetRelativePos(100.f, 500.f, 0.f);

	/*m_TileMap->CreateTile<CTile>(Tile_Shape::Rect, 100, 100, Vector2(64.f, 64.f));

	m_TileMap->SetMaterial(0, "MainMapRect");
	m_TileMap->SetFrameMax(1, 5);
	m_TileMap->SetTileDefaultFrame(0, 0);*/

	return true;
}

void CMainMap::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CMainMap::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CMainMap::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CMainMap::Render(float DeltaTime)
{
	CGameObject::Render(DeltaTime);
}

CMainMap* CMainMap::Clone()
{
	return new CMainMap(*this);
}
