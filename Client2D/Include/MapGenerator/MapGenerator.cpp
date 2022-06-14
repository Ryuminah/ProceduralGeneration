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
	SAFE_DELETE(m_pTileFinder);
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
	m_pTileFinder = new CTileFinder(this);

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
	case TILE_STATE::CLEAR:
		ClearAll();
		break;
	default:
		break;
	}

	// 각 상태가 끝난 이후 타일 상태를 항상 업데이트 해준다.
	UpdateTileStateData();
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

	for (int x = 0; x < m_MapSizeX; ++x)
	{
		for (int y = 0; y < m_MapSizeY; ++y)
		{
			ChangeTileState(Vector2(x, y), TILE_STATE::LAND);
		}
	}

	clock_t start = clock(); // 시작 시 간 저장

	// 맵 자동화 알고리즘
	CellularAutomata();

	clock_t end = clock();
	double clockTime = (double)(end - start) / CLOCKS_PER_SEC;
	char buffer[BUFSIZ];
	sprintf_s(buffer, "%f", clockTime);
	CEngine::GetInst()->AddDebugLog(buffer);

	for (int x = 0; x < m_OutLineX; ++x)
	{
		for (int y = 0; y < m_MapSizeY; ++y)
		{
			Vector2 LineStart = Vector2(x, y);
			Vector2 LineEnd = Vector2(m_MapSizeX - 1 - x, y);

			ChangeTileState(LineStart, TILE_STATE::SEA);
			ChangeTileState(LineEnd, TILE_STATE::SEA);

		}
	}

	for (int y = 0; y < m_OutLineY; ++y)
	{
		for (int x = 0; x < m_MapSizeX; ++x)
		{
			Vector2 LineStart = Vector2(x, y);
			Vector2 LineEnd = Vector2(x, m_MapSizeY - 1 - y);

			ChangeTileState(LineStart, TILE_STATE::SEA);
			ChangeTileState(LineEnd, TILE_STATE::SEA);

		}
	}
}

void CMapGenerator::GenerateLand()
{
	// 이것도 스무딩 강도 정하기

	// 1. 호수 없이 전부 땅으로 메움
	// 옵션을 전부 멤버 변수로. . . 추가 .. .

	// 물타일의 주변 타일이 땅이면 땅으로 채운다..

	// 
	// 주변 타일 4개이상 땅이면 땅으로 바꿈 (최대한 땅으로 바꿈)
	int InnerTileLineX = m_MapSizeX - m_OutLineX;
	int InnerTileLineY = m_MapSizeY - m_OutLineY;

	for (int x = m_OutLineX; x < InnerTileLineX; ++x)
	{
		for (int y = m_OutLineY; y < InnerTileLineY; ++y)
		{
			if (m_TileData[x][y] == TILE_STATE::SEA)
			{
				int NearSeaCount = m_pTileFinder->Check_NearTileState8(x, y, TILE_STATE::SEA);

				if (NearSeaCount < 4)
				{
					ChangeTileState(Vector2(x, y), TILE_STATE::LAND);
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
	std::vector<Vector2> vecLandTile = m_pTileFinder->Get_AreaBorder(TILE_STATE::LAND);
	std::vector<Vector2> vecNewCoastTile;

	for (size_t i = 0; i < vecLandTile.size(); ++i)
	{
		Vector2 CoastTile = vecLandTile[i];

		ChangeTileState(CoastTile, TILE_STATE::COAST);
		vecNewCoastTile.push_back(CoastTile);
	}

	int keyIndex = 0;
	std::map<int, Vector2> mapRandomSandTile;

	// 샌드 만들고 랜덤으로 ...
	for (size_t i = 0; i < vecNewCoastTile.size(); ++i)
	{
		Vector2 currentSandTile = vecNewCoastTile[i];

		// 가장자리의 가장자리를 가져왕 , , , ,,(TILE_STATE에 제작)
		for (int nearX = currentSandTile.x - 1; nearX <= currentSandTile.x + 1; ++nearX)
		{
			if (nearX == currentSandTile.x)
			{
				continue;
			}

			// 맵 범위 이내의 타일이 모래가 아니라면 저장한다
			if ((nearX >= 0 && nearX < m_MapSizeX) &&
				m_TileData[nearX][currentSandTile.y])
			{
				m_TileData[nearX][currentSandTile.y] != TILE_STATE::COAST;
				mapRandomSandTile.insert(std::pair<int, Vector2>(keyIndex, Vector2(nearX, currentSandTile.y)));

				++keyIndex;
			}
		}

		for (int nearY = currentSandTile.y - 1; nearY <= currentSandTile.y + 1; nearY++)
		{
			if (nearY == currentSandTile.y)
			{
				continue;
			}

			// 맵 범위 체크
			if ((nearY >= 0 && nearY < m_MapSizeY) &&
				m_TileData[currentSandTile.x][nearY])
			{
				m_TileData[currentSandTile.x][nearY] != TILE_STATE::COAST;
				mapRandomSandTile.insert(std::pair<int, Vector2>(keyIndex, Vector2(currentSandTile.x, nearY)));
				++keyIndex;
			}
		}
	}

	// 랜덤으로 뽑아서 . .. . . 샌드 타일 만들기
	std::random_device randomDevice;
	std::mt19937_64 gen(randomDevice());

	int RandomTileCount = mapRandomSandTile.size() * 0.5f;
	int RandomSeed = 0;

	while (RandomTileCount)
	{
		// seed값을 이용하여 타일을 랜덤한 위치에 생성
		// 남은 타일의 갯수 중 인덱스 선정
		std::uniform_int_distribution<int> dist(0, mapRandomSandTile.size());
		RandomSeed = dist(randomDevice);

		Vector2 TileIndex = mapRandomSandTile[RandomSeed];

		//ChangeTileImage(TileIndex, TILE_State::COAST);
		//m_AllTileStateData.insert(std::pair<TILE_State, std::vector<Vector2>>(TILE_State::COAST, vecCoastTile));
		vecNewCoastTile.push_back(TileIndex);

		// 랜덤 인덱스의 value를 End로 교체, 가장 마지막 값 삭제
		auto iterEnd = mapRandomSandTile.end();
		mapRandomSandTile[RandomSeed] = (--iterEnd)->second;
		mapRandomSandTile.erase(iterEnd);

		--RandomTileCount;
	}

	//m_AllTileStateData.insert(std::pair<TILE_State, std::vector<Vector2>>(TILE_State::COAST, vecCoastTile));
}

void CMapGenerator::GenerateLake()
{
	// 호수 타일 생성
}

void CMapGenerator::GenerateForest()
{
	int Index = 0;
	int randomCount = 2;

	// 매직 넘버 꼭 바꿔,, 바꿔,,, ㅠ
	int forestX = (rand() % 15) + 7;
	int forestY = (rand() % 16) + 8;

	// 숲 시작지점의 인덱스를 랜덤으로 고름
	Vector2 forestStart;
	int startIndex = rand() % m_TileStateData[TILE_STATE::LAND].size();
	forestStart = m_TileStateData[TILE_STATE::LAND][startIndex];

	// 숲의 마지막 지점
	Vector2 forestEnd;
	forestEnd.x = forestStart.x + forestX;
	forestEnd.y = forestStart.y + forestY;

	// 사각형의 숲 만들기
	std::vector<Vector2> vecForestTile;
	for (int x = forestStart.x; x < forestEnd.x; ++x)
	{
		for (int y = forestStart.y; y < forestEnd.y; ++y)
		{
			// 땅 타일일 경우에만 숲을 추가한다
			if (x >= 0 && x < m_MapSizeX &&
				y >= 0 && y < m_MapSizeY &&
				m_TileData[x][y] == TILE_STATE::LAND)
			{
				ChangeTileState(Vector2(x, y), TILE_STATE::FOREST);
				vecForestTile.push_back(Vector2(x, y));
			}
		}
	}

	//UpdateTileStateData()
	// 외곽 스무딩
	// Outline을 모아서 뜬 ,,,

	//std::vector<Vector2> vecSmoothTile = m_pTileFinder->Get_AreaBorder(TILE_STATE::FOREST);
	//std::vector<Vector2> vecOutlineTile = m_pTileFinder->Get_OutlineTiles(vecSmoothTile);

	//for (size_t i = 0; i < vecOutlineTile.size(); ++i)
	//{
	//	// 테두리 중 숲 타일이 아니라면 경계의 아웃라인으로 간주하고 스무딩 라인으로 추가한다.
	//	if (!m_pTileFinder->Check_TileState(vecOutlineTile[i], TILE_STATE::FOREST))
	//	{
	//		vecSmoothTile.push_back(vecOutlineTile[i]);
	//	}
	//}

	//for (size_t i = 0; i < vecSmoothTile.size(); ++i)
	//{
	//	bool changeForest  = rand() % 1;

	//	switch (changeForest)
	//	{
	//	case true:
	//		ChangeTileState(vecSmoothTile[i], TILE_STATE::FOREST);
	//		break;

	//	case false:
	//		ChangeTileState(vecSmoothTile[i], TILE_STATE::LAND);
	//		break;

	//	default:
	//		break;
	//	}
	//}
}

void CMapGenerator::ClearAll()
{
	m_TileData.clear();

	/*for (int x = 0; x < m_MapSizeX; ++x)
	{
		for (int y = 0; y < m_MapSizeY; ++y)
		{
			ChangeTileData(Vector2(x, y), TILE_STATE::LAND);
		}
	}*/

	GenerateBase();
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
	//
	SmoothMap();
	//for (int i = 0; i < 2; i++)					// 우선 5번 정도를 횟수 최대로,,
	//{
	//}
}

void CMapGenerator::SmoothMap()
{

	for (int x = 0; x < m_MapSizeX - m_OutLineX; ++x)
	{
		int wallCount = 0;

		for (int y = 0; y < m_MapSizeY - m_OutLineY; ++y)
		{
			int NearSeaCount = m_pTileFinder->Check_NearSeaTile8(x, y);

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
	if (!m_pTileFinder->IsExistTile(tileIndex))
	{
		return;
	}

	CTile* pTile = m_pRandomMap->m_MapComponent->GetTile(tileIndex * TILE_SIZE_SMALL);
	pTile->SetFrameStart(tileState * TILE_SIZE_SMALL, 0.f);
	pTile->SetFrameEnd((tileState + 1) * TILE_SIZE_SMALL, TILE_SIZE_SMALL);

	// 타일의 상태가 확실히 변경 되었을 때 정보를 교체한다
	ChangeTileData(tileIndex, tileState);
}

void CMapGenerator::ChangeTileData(Vector2 tileIndex, TILE_STATE tileState)
{
	m_TileData[(int)tileIndex.x][(int)tileIndex.y] = tileState;
}

void CMapGenerator::UpdateTileStateData()
{
	m_TileStateData.clear();

	std::vector<Vector2> vecLandTile;
	std::vector<Vector2> vecSeaTile;
	std::vector<Vector2> vecCoastTile;
	std::vector<Vector2> vecLakeTile;
	std::vector<Vector2> vecForestTile;

	// 전체 타일정보를 체크해서 분류 별로 한번 더 저장
	for (int x = 0; x < m_MapSizeX; ++x)
	{
		for (int y = 0; y < m_MapSizeY; ++y)
		{
			TILE_STATE currentTileState = m_TileData[x][y];

			switch (currentTileState)
			{
			case LAND:
				vecLandTile.push_back(Vector2(x, y));
				break;
			case SEA:
				vecSeaTile.push_back(Vector2(x, y));
				break;
			case COAST:
				vecCoastTile.push_back(Vector2(x, y));
				break;
			case LAKE:
				vecLakeTile.push_back(Vector2(x, y));
				break;
			case FOREST:
				vecForestTile.push_back(Vector2(x, y));
				break;
			default:
				break;
			}
		}
	}

	std::vector<Vector2> allTileState[TILE_STATE_SIZE] = { vecLandTile ,vecSeaTile, vecCoastTile, vecLakeTile, vecForestTile };

	for (int i = 0; i < TILE_STATE_SIZE; i++)
	{
		TILE_STATE tileState = static_cast<TILE_STATE>(i);
		m_TileStateData[tileState] = allTileState[i];
		//m_TileStateData.insert({ tileState , allTileState[i] });
	}
}

void CMapGenerator::CreateRandom()
{

}
