#include <time.h>
#include <random>
#include "MapGenerator.h"
#include "RandomMap.h"
#include "Engine.h"
#include "Timer.h"
#include "Input.h"

CMapGenerator::CMapGenerator() : m_IsGenerateWorldEnd(false)
{
}

CMapGenerator::~CMapGenerator()
{
	// 여기 소멸자 호풀 안됨 끄응
	//int a = 0;
}

bool CMapGenerator::Init()
{

	return true;
}

bool CMapGenerator::Init(CRandomMap* pRandomMap)
{
	if (!m_pRandomMap)
	{
		m_pRandomMap = pRandomMap;
	}

	// CMapGenerator에서 RandomMap 객체의 맵을 생성해준다.
	CTileMapComponent* pMapComponent = m_pRandomMap->CreateSceneComponent<CTileMapComponent>("RandomMap");
	m_pRandomMap->m_MapComponent = pMapComponent;
	m_pRandomMap->SetRootComponent(pMapComponent);

	// Tile들을 생성한다.
	pMapComponent->CreateTile<CTile>(Tile_Shape::Rect, m_pRandomMap->m_MapSizeX, m_pRandomMap->m_MapSizeY, Vector2(TILE_SIZE_SMALL, TILE_SIZE_SMALL));

	return true;
}

// virtual
void CMapGenerator::GenerateWorld(LAND_STATE _tileState)
{
	switch (_tileState)
	{
	case LAND_STATE::BASE:
		GenerateBase();
		break;
	case LAND_STATE::LAND:
		GenerateLand();
		break;
	case LAND_STATE::SEA:
		GenerateSea();
		break;
	case LAND_STATE::COAST:
		GenerateCoast();
		break;
	case LAND_STATE::LAKE:
		GenerateLake();
		break;
	case LAND_STATE::FOREST:
		GenerateForest();
		break;
	default:
		break;
	}
}

void CMapGenerator::GenerateVegetation(LAND_STATE _landState)
{
	if (!m_IsGenerateWorldEnd)
	{
		return;
	}


	// 현재까지 생성된 타일 정보를 기반으로 
	// 식생을 생성한다.
}


// Member Function
void CMapGenerator::GenerateBase()
{
	// 기본 맵 생성 로직
	// 시드값을 이용해서 맵의 기반을 만듦

	CEngine::GetInst()->OnDebugLog();
	m_IsGenerateWorldEnd = false;

	for (int x = 0; x < m_pRandomMap->m_MapSizeX; ++x)
	{
		std::vector<LAND_STATE> tileData;

		for (int y = 0; y < m_pRandomMap->m_MapSizeY; ++y)
		{
			tileData.push_back(LAND_STATE::LAND);
		}

		m_TileData.push_back(tileData);
	}

	clock_t start = clock(); // 시작 시간 저장

	CellularAutomata();

	clock_t end = clock();
	double clockTime = (double)(end - start) / CLOCKS_PER_SEC;
	char buffer[BUFSIZ];
	sprintf_s(buffer, "%f", clockTime);
}

void CMapGenerator::GenerateLand()
{
	// Land 옵션 추가... 어떻게 ㄱㅡ...
}

void CMapGenerator::GenerateSea()
{
	
}

void CMapGenerator::GenerateCoast()
{
	// 해안가 생성 코드
}

void CMapGenerator::GenerateLake()
{
	// 호수 타일 생성
}

void CMapGenerator::GenerateForest()
{
	// 호수 근처에 숲 지역을 생성.
}

void CMapGenerator::CellularAutomata()
{
}
