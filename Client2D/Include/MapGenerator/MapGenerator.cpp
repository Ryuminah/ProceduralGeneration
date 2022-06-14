#include <set>
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
	m_OutLineX(2), m_OutLineY(2),
	m_ForestMinX(7), m_ForestMinY(8), m_ForestMaxX(15), m_ForestMaxY(16)

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

	// GenerateOption ����
	//CGenerateOptionManager::GetInst()->CreateGenerateOption("Base");

	// CMapGenerator���� RandomMap ��ü�� ���� �������ش�. (���� ����ĥ �ð��� ��� ��)
	CTileMapComponent* pMapComponent = m_pRandomMap->CreateSceneComponent<CTileMapComponent>("RandomMap");
	m_pRandomMap->m_MapComponent = pMapComponent;
	m_pRandomMap->SetRootComponent(pMapComponent);

	// Tile���� �����Ѵ�.
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

	// �� ���°� ���� ���� Ÿ�� ���¸� �׻� ������Ʈ ���ش�.
	UpdateTileStateData();
}

void CMapGenerator::GenerateVegetation(TILE_STATE _landState)
{
	if (!m_IsGenerateWorldEnd)
	{
		return;
	}


	// ������� ������ Ÿ�� ������ ������� 
	// �Ļ��� �����Ѵ�.
	// �������� ���� �����ھƹ��� , , ,,
}

// Member Function
void CMapGenerator::GenerateBase()
{
	// �⺻ �� ���� ����
	// �õ尪�� �̿��ؼ� ���� ����� ����

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

	clock_t start = clock(); // ���� �� �� ����

	// �� �ڵ�ȭ �˰���
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
	// �̰͵� ������ ���� ���ϱ�

	// 1. ȣ�� ���� ���� ������ �޿�
	// �ɼ��� ���� ��� ������. . . �߰� .. .

	// ��Ÿ���� �ֺ� Ÿ���� ���̸� ������ ä���..

	// 
	// �ֺ� Ÿ�� 4���̻� ���̸� ������ �ٲ� (�ִ��� ������ �ٲ�)
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
	// �� �ɼ��� ���߿� �� ���� �ɼ����� ����
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

	// ���� ����� �������� ...
	for (size_t i = 0; i < vecNewCoastTile.size(); ++i)
	{
		Vector2 currentSandTile = vecNewCoastTile[i];

		// �����ڸ��� �����ڸ��� ������ , , , ,,(TILE_STATE�� ����)
		for (int nearX = currentSandTile.x - 1; nearX <= currentSandTile.x + 1; ++nearX)
		{
			if (nearX == currentSandTile.x)
			{
				continue;
			}

			// �� ���� �̳��� Ÿ���� �𷡰� �ƴ϶�� �����Ѵ�
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

			// �� ���� üũ
			if ((nearY >= 0 && nearY < m_MapSizeY) &&
				m_TileData[currentSandTile.x][nearY])
			{
				m_TileData[currentSandTile.x][nearY] != TILE_STATE::COAST;
				mapRandomSandTile.insert(std::pair<int, Vector2>(keyIndex, Vector2(currentSandTile.x, nearY)));
				++keyIndex;
			}
		}
	}

	// �������� �̾Ƽ� . .. . . ���� Ÿ�� �����
	std::random_device randomDevice;
	std::mt19937_64 gen(randomDevice());

	int RandomTileCount = mapRandomSandTile.size() * 0.5f;
	int RandomSeed = 0;

	while (RandomTileCount)
	{
		// seed���� �̿��Ͽ� Ÿ���� ������ ��ġ�� ����
		// ���� Ÿ���� ���� �� �ε��� ����
		std::uniform_int_distribution<int> dist(0, mapRandomSandTile.size());
		RandomSeed = dist(randomDevice);

		Vector2 TileIndex = mapRandomSandTile[RandomSeed];

		//ChangeTileImage(TileIndex, TILE_State::COAST);
		//m_AllTileStateData.insert(std::pair<TILE_State, std::vector<Vector2>>(TILE_State::COAST, vecCoastTile));
		vecNewCoastTile.push_back(TileIndex);

		// ���� �ε����� value�� End�� ��ü, ���� ������ �� ����
		auto iterEnd = mapRandomSandTile.end();
		mapRandomSandTile[RandomSeed] = (--iterEnd)->second;
		mapRandomSandTile.erase(iterEnd);

		--RandomTileCount;
	}

	//m_AllTileStateData.insert(std::pair<TILE_State, std::vector<Vector2>>(TILE_State::COAST, vecCoastTile));
}

void CMapGenerator::GenerateLake()
{
	// ȣ�� Ÿ�� ����
}

void CMapGenerator::GenerateForest()
{
	// �� ������ �����ǹǷ�, ���� �������� �ʴ´ٸ� �Լ� ����
	int LandTilesCount = m_TileStateData[TILE_STATE::LAND].size();

	if (LandTilesCount == 0)
	{
		return;
	}

	int Index = 0;
	int randomCount = 2;

	// ���� �ѹ� �� �ٲ�,, �ٲ�,,, ��
	int forestX = (rand() % m_ForestMaxX) + m_ForestMinX;
	int forestY = (rand() % m_ForestMaxY) + m_ForestMinY;

	// �� ���������� �ε����� �������� ��
	int startIndex = rand() % LandTilesCount;
	Vector2 forestStart = m_TileStateData[TILE_STATE::LAND][startIndex];

	// ���� ������ ����
	Vector2 forestEnd;
	forestEnd.x = forestStart.x + forestX;
	forestEnd.y = forestStart.y + forestY;

	// �簢���� �� �����
	std::vector<Vector2> vecForestTile;
	for (int x = forestStart.x; x < forestEnd.x; ++x)
	{
		for (int y = forestStart.y; y < forestEnd.y; ++y)
		{
			// �� Ÿ���� ��쿡�� ���� �߰��Ѵ�
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
	// �ܰ� ������
	// Outline�� ��Ƽ� �� ,,,

	//std::vector<Vector2> vecSmoothTile = m_pTileFinder->Get_AreaBorder(TILE_STATE::FOREST);
	//std::vector<Vector2> vecOutlineTile = m_pTileFinder->Get_OutlineTiles(vecSmoothTile);

	//for (size_t i = 0; i < vecOutlineTile.size(); ++i)
	//{
	//	// �׵θ� �� �� Ÿ���� �ƴ϶�� ����� �ƿ��������� �����ϰ� ������ �������� �߰��Ѵ�.
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
	// �� ũ�⸸ŭ ����Ʈ�� �ʿ� �����س���
	// ��÷ ��ȣ�� ��带 ����
	std::random_device randomDevice;
	std::mt19937_64 gen(randomDevice());

	std::unordered_map<int, Vector2> MapIndex;
	std::set<int> MapIndexKey;				// Ű�� �����س���
	std::vector<Vector2> RandomIndex;

	int Index = 0;

	for (int x = 0; x < m_MapSizeX; ++x)
	{
		for (int y = 0; y < m_MapSizeY; ++y)
		{
			MapIndex[Index] = Vector2((float)x, (float)y);
			MapIndexKey.insert(Index);
			++Index;
		}
	}

	int RandomTileCount = (m_MapSizeX * m_MapSizeY) * 0.52f;
	int RandomSeed = 0;

	while (RandomTileCount)
	{
		// seed���� �̿��Ͽ� Ÿ���� ������ ��ġ�� ����
		// ���� Ÿ���� ���� �� �ε��� ����
		std::uniform_int_distribution<int> dist(0, MapIndex.size());
		RandomSeed = dist(randomDevice);

		Vector2 TileIndex = MapIndex[RandomSeed];

		// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (���� ����)
		ChangeTileState(TileIndex, TILE_STATE::SEA);

		// ���� �ε����� value�� End�� ��ü, ���� ������ �� ����
		auto iterEnd = MapIndex.end();
		MapIndex[RandomSeed] = (--iterEnd)->second;
		MapIndex.erase(iterEnd);

		--RandomTileCount;

		// Fixed to the right

	}

	// Smooth Map
	// ������ ������ ���� �� �ְ� �Ѵ�. Min / Normal /Max
	// 1. �ִ� ���������� Ƚ��(���̻� �ʿ� ��ȭ�� ����������) �� üũ�� �� Ƚ�� 
	// 2. �� ������ Ÿ�� ���¸� ���� ������ �� �߰��߰� Ȯ�� �Ǳ� ������ ���� ���¸� �� �� �ֵ��� �Ѵ� .. .. . . .
	//
	SmoothMap();
}

void CMapGenerator::SmoothMap()
{
	for (int x = 0; x < m_MapSizeX - m_OutLineX; ++x)
	{
		for (int y = 0; y < m_MapSizeY - m_OutLineY; ++y)
		{
			int NearSeaCount = m_pTileFinder->Check_NearSeaTile8(x, y);

			if (NearSeaCount > 4)
			{
				// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (���� ����)
				ChangeTileState(Vector2(x, y), TILE_STATE::SEA);
			}

			else if (NearSeaCount < 4)
			{
				// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (������ ����)
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

	// �ܺο��� �����ؼ� �׷��� ���� ��������;
	// CTile �����ϰ� �ִ°͵� ����ϱ�.. . . (�ӵ� ���� ���� ��)
	CTile* pTile = m_pRandomMap->m_MapComponent->GetTile(tileIndex * TILE_SIZE_SMALL);
	pTile->SetFrameStart(tileState * TILE_SIZE_SMALL, 0.f);
	pTile->SetFrameEnd((tileState + 1) * TILE_SIZE_SMALL, TILE_SIZE_SMALL);

	// Ÿ���� ���°� Ȯ���� ���� �Ǿ��� �� ������ ��ü�Ѵ�
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

	for (int x = 0; x < m_MapSizeX; ++x)
	{
		for (int y = 0; y < m_MapSizeY; ++y)
		{
			TILE_STATE currentTileState = m_TileData[x][y];
			Vector2 currentIndex = Vector2((float)x, (float)y);

			switch (currentTileState)
			{
			case LAND:
				vecLandTile.push_back(currentIndex);
				break;
			case SEA:
				vecSeaTile.push_back(currentIndex);
				break;
			case COAST:
				vecCoastTile.push_back(currentIndex);
				break;
			case LAKE:
				vecLakeTile.push_back(currentIndex);
				break;
			case FOREST:
				vecForestTile.push_back(currentIndex);
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
