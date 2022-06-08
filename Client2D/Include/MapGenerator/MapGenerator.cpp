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

	// GenerateOption ����
	//CGenerateOptionManager::GetInst()->CreateGenerateOption("Base");

	// CMapGenerator���� RandomMap ��ü�� ���� �������ش�. (���� ����ĥ �ð��� ��� ��)
	CTileMapComponent* pMapComponent = m_pRandomMap->CreateSceneComponent<CTileMapComponent>("RandomMap");
	m_pRandomMap->m_MapComponent = pMapComponent;
	m_pRandomMap->SetRootComponent(pMapComponent);

	// Tile���� �����Ѵ�.
	pMapComponent->CreateTile<CTile>(Tile_Shape::Rect, m_pRandomMap->m_MapSizeX, m_pRandomMap->m_MapSizeY, Vector2(TILE_SIZE_SMALL, TILE_SIZE_SMALL));
	CTileFinder tileFinder = CTileFinder(this);		// �̰� ����?
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

	clock_t start = clock(); // ���� �ð� ����

	// �� �ڵ�ȭ �˰���
	CellularAutomata();

	clock_t end = clock();
	double clockTime = (double)(end - start) / CLOCKS_PER_SEC;
	char buffer[BUFSIZ];
	sprintf_s(buffer, "%f", clockTime);
	CEngine::GetInst()->AddDebugLog(buffer);


}

void CMapGenerator::GenerateLand()
{
	// 1. ȣ�� ���� ���� ������ �޿�
	// �ɼ��� ���� ��� ������. . . �߰� .. .

	// ��Ÿ���� �ֺ� Ÿ���� ���̸� ������ ä���..

	// 
	// �ֺ� Ÿ�� 4���̻� ���̸� ������ �ٲ� (�ִ��� ������ �ٲ�)
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
	// ������ ���� ������� �ؾȰ� ����

	//std::vector<Vector2> vecSeaTile;


	//for (int x = 0; x < m_MapSizeX; ++x)
	//{
	//	for (int y = 0; y < m_MapSizeY; ++y)
	//	{
	//		if (m_TileData[x][y] == TILE_State::SEA)
	//		{
	//			vecSeaTile.push_back(Vector2(x, y));
	//		}

	//		else
	//		{
	//			vecLandTile.push_back(Vector2(x, y));

	//		}
	//	}
	//}

	std::vector<Vector2> vecLandTile = m_TileStateData[TILE_STATE::LAND];
	std::vector<Vector2> vecNewCoastTile;

	// Land Tile �� �����ڸ��� �ִ� Ÿ���� �𷡷� �ٲ�
	//for (size_t i = 0; i < vecLandTile.size(); ++i)
	//{
	//	Vector2 Index = vecLandTile[i];
	//	int NearSeaCount = m_pTileFinder->Check_NearTileState4(Index.x, Index.y, TILE_STATE::SEA);

	//	if (NearSeaCount > 0)
	//	{
	//		ChangeTileState(Index, TILE_STATE::COAST);
	//		vecNewCoastTile.push_back(Index);
	//	}
	//}

	std::vector<Vector2> vecLandTile = m_pTileFinder->GetAreaBorder(TILE_STATE::LAND);

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
	// ȣ�� ��ó�� �� ������ ����.
}

void CMapGenerator::CellularAutomata()
{
	// �� ũ�⸸ŭ ����Ʈ�� �ʿ� �����س���
	// ��÷ ��ȣ�� ��带 ����
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
	}

	// Smooth Map
	// ������ ������ ���� �� �ְ� �Ѵ�. Min / Normal /Max
	// 1. �ִ� ���������� Ƚ��(���̻� �ʿ� ��ȭ�� ����������) �� üũ�� �� Ƚ�� 
	// 2. �� ������ Ÿ�� ���¸� ���� ������ �� �߰��߰� Ȯ�� �Ǳ� ������ ���� ���¸� �� �� �ֵ��� �Ѵ� .. .. . . .
	
	for (int i = 0; i < 5; i++)					// �켱 5�� ������ Ƚ�� �ִ��,,
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

	CTile* pTile = m_pRandomMap->m_MapComponent->GetTile(tileIndex * TILE_SIZE_SMALL);
	pTile->SetFrameStart(tileState * TILE_SIZE_SMALL, 0.f);
	pTile->SetFrameEnd((tileState + 1) * TILE_SIZE_SMALL, TILE_SIZE_SMALL);

	// Ÿ���� ���°� Ȯ���� ���� �Ǿ��� �� ������ ��ü�Ѵ�
	ChangeTileData(tileIndex, tileState);
}

void CMapGenerator::ChangeTileData(Vector2 tileIndex, TILE_STATE tileState)
{
	// �����Ͱ� �ִ��� ������ Ȯ�� ��, �̹� �ִ� �����Ͷ�� �ٲ� ������ �����Ѵ�.
	// ������ �߰�...

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

	// ��ü Ÿ�������� üũ�ؼ� ����
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
