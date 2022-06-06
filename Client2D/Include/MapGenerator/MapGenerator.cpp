#include <time.h>
#include <random>
#include "GenerateOptionManager.h"
#include "MapGenerator.h"
#include "TileFinder.h"
#include "RandomMap.h"
#include "Engine.h"
#include "Timer.h"
#include "Input.h"

CMapGenerator::CMapGenerator() : 
	m_IsGenerateWorldEnd(false),
	m_pRandomMap(nullptr),
	m_pTileFinder(nullptr),
	m_MapSizeX(50), m_MapSizeY(50), 
	m_OutLineX(2), m_OutLineY(2)
{
}

CMapGenerator::~CMapGenerator()
{

}

bool CMapGenerator::Init(CRandomMap* pRandomMap)
{
	if (!m_pRandomMap)
	{
		m_pRandomMap = pRandomMap;
	}

	// GenerateOption 생성
	//CGenerateOptionManager::GetInst()->CreateGenerateOption("Base");

	// CMapGenerator에서 RandomMap 객체의 맵을 생성해준다. (엔진 뜯어고칠 시간이 없어서 ㅠ)
	CTileMapComponent* pMapComponent = m_pRandomMap->CreateSceneComponent<CTileMapComponent>("RandomMap");
	m_pRandomMap->m_MapComponent = pMapComponent;
	m_pRandomMap->SetRootComponent(pMapComponent);

	// Tile들을 생성한다.
	pMapComponent->CreateTile<CTile>(Tile_Shape::Rect, m_pRandomMap->m_MapSizeX, m_pRandomMap->m_MapSizeY, Vector2(TILE_SIZE_SMALL, TILE_SIZE_SMALL));
	CTileFinder tileFinder = CTileFinder(this);		// 이거 동적?
	m_pTileFinder = &tileFinder;

	return true;
}

void CMapGenerator::GenerateWorld(TILE_STATE _tileState)
{
	switch (_tileState)
	{
	case TILE_STATE::BASE:
		GenerateBase();
		break;
	case TILE_STATE::LAND:
		GenerateLand();
		break;
	case TILE_STATE::SEA:
		GenerateSea();
		break;
	case TILE_STATE::COAST:
		GenerateCoast();
		break;
	case TILE_STATE::LAKE:
		GenerateLake();
		break;
	case TILE_STATE::FOREST:
		GenerateForest();
		break;
	default:
		break;
	}
}

void CMapGenerator::GenerateVegetation(TILE_STATE _landState)
{
	if (!m_IsGenerateWorldEnd)
	{
		return;
	}


	// 현재까지 생성된 타일 정보를 기반으로 
	// 식생을 생성한다.
	// 랜덤으로 갖다 때려박아버려 , , ,,
}


// Member Function
void CMapGenerator::GenerateBase()
{
	// 기본 맵 생성 로직
	// 시드값을 이용해서 맵의 기반을 만듦

	CEngine::GetInst()->OnDebugLog();
	m_IsGenerateWorldEnd = false;

	for (int x = 0; x < m_MapSizeX; ++x)
	{
		std::vector<TILE_STATE> tileData;

		for (int y = 0; y < m_MapSizeY; ++y)
		{
			tileData.push_back(TILE_STATE::LAND);
		}

		m_TileData.push_back(tileData);
	}

	clock_t start = clock(); // 시작 시간 저장

	// 맵 자동화 알고리즘
	CellularAutomata();

	clock_t end = clock();
	double clockTime = (double)(end - start) / CLOCKS_PER_SEC;
	char buffer[BUFSIZ];
	sprintf_s(buffer, "%f", clockTime);
	CEngine::GetInst()->AddDebugLog(buffer);

}

void CMapGenerator::GenerateLand()
{
	// 1. 호수 없이 전부 땅으로 메움
	// 옵션을 전부 멤버 변수로. . . 추가 .. .

	// 물타일의 주변 타일이 땅이면 땅으로 채운다..

	// 
	// 주변 타일 4개이상 땅이면 땅으로 바꿈 (최대한 땅으로 바꿈)
	int InnerTileLineX = m_MapSizeX - m_OutLineX;
	int InnerTileLineY = m_MapSizeY - m_OutLineY;

	for (int x = m_OutLineX; x < InnerTileLineX; ++x)
	{
		for (int y = m_OutLineY; y < InnerTileLineY - 3; ++y)
		{
			if (m_TileData[x][y] == TILE_STATE::SEA)
			{
				int NearSeaCount = m_pTileFinder->Check_NearTileState8(x, y, TILE_STATE::SEA);

				if (NearSeaCount <= 4)
				{
					m_pTileFinder->Check_NearTileState8(x, y, TILE_STATE::LAND);
					//m_AllTileStateData[TILE_STATE::LAND].push_back(Vector2(x, y));

				}
			}
		}
	}

}

void CMapGenerator::GenerateSea()
{
	// 이 옵션은 나중에 섬 생성 옵션으로 변경
}

void CMapGenerator::GenerateCoast()
{
	// 생성된 땅을 기반으로 해안가 생성
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
	// 맵 크기만큼 리스트나 맵에 저장해놓고
	// 당첨 번호의 노드를 삭제
	std::random_device randomDevice;
	std::mt19937_64 gen(randomDevice());
	std::unordered_map<int, Vector2> MapIndex;
	int Index = 0;

	for (int x = 0; x < m_MapSizeX; ++x)
	{
		for (int y = 0; y < m_MapSizeY; ++y)
		{
			MapIndex[Index] = Vector2((float)x, (float)y);
			++Index;
		}
	}

	int RandomTileCount = (m_MapSizeX * m_MapSizeY) * 0.42f;
	int RandomSeed = 0;

	while (RandomTileCount)
	{
		// seed값을 이용하여 타일을 랜덤한 위치에 생성
		// 남은 타일의 갯수 중 인덱스 선정
		std::uniform_int_distribution<int> dist(0, MapIndex.size());
		RandomSeed = dist(randomDevice);

		Vector2 TileIndex = MapIndex[RandomSeed];

		// 해당 부분의 타일만 UV좌표를 변경 (물로 변경)
		ChangeTileState(TileIndex, TILE_STATE::SEA);

		// 랜덤 인덱스의 value를 End로 교체, 가장 마지막 값 삭제
		auto iterEnd = MapIndex.end();
		MapIndex[RandomSeed] = (--iterEnd)->second;
		MapIndex.erase(iterEnd);

		--RandomTileCount;
	}

	// Smooth Map
	// 스무딩 강도를 정할 수 있게 한다. Min / Normal /Max
	// 1. 최대 강도까지의 횟수(더이상 맵에 변화가 없을때까지) 를 체크한 후 횟수 
	// 2. 각 레벨의 타일 상태를 전부 저장한 후 중간중간 확정 되기 전까지 맵의 상태를 볼 수 있도록 한다 .. .. . . .
	
	for (int i = 0; i < 5; i++)					// 우선 5번 정도를 횟수 최대로,,
	{
		SmoothMap();
	}
}

void CMapGenerator::SmoothMap()
{
	for (int x = 0; x < m_MapSizeX; ++x)
	{
		for (int y = 0; y < m_MapSizeY; ++y)
		{
			int NearSeaCount = m_pTileFinder->Check_NearTileState4(x, y, TILE_STATE::SEA);

			if (NearSeaCount > 4)
			{
				// 해당 부분의 타일만 UV좌표를 변경 (물로 변경)
				ChangeTileState(Vector2(x, y), TILE_STATE::SEA);
			}

			else if (NearSeaCount < 4)
			{
				// 해당 부분의 타일만 UV좌표를 변경 (땅으로 변경)
				ChangeTileState(Vector2(x, y), TILE_STATE::LAND);
			}
		}
	}
}


void CMapGenerator::ChangeTileState(Vector2 tileIndex, TILE_STATE tileState)
{
	if (!m_pTileFinder->Check_ExistTile(tileIndex))
	{
		return;
	}

	CTile* pTile = m_pRandomMap->m_MapComponent->GetTile(tileIndex * TILE_SIZE_SMALL);
	pTile->SetFrameStart(tileState * TILE_SIZE_SMALL, 0.f);
	pTile->SetFrameEnd((tileState + 1) * TILE_SIZE_SMALL, TILE_SIZE_SMALL);

	// 타일의 상태가 확실히 변경 되었을 때 정보를 저장한다
	ChangeTileState(tileIndex, tileState);
}

void CMapGenerator::ChangeTileStateData(Vector2 tileIndex, TILE_STATE tileState)
{
	// 데이터가 있는지 없는지 확인 후, 이미 있는 데이터라면 바뀐 정보로 갱신한다.
	// 데이터 추가...

	m_TileData[(int)tileIndex.x][(int)tileIndex.y] = tileState;
}

void CMapGenerator::CreateRandom()
{
}
