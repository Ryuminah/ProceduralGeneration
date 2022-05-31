#include <time.h>
#include <random>
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
	m_SeaEndLineX(2), m_SeaEndLineY(2)
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

	// CMapGenerator���� RandomMap ��ü�� ���� �������ش�. (���� ����ĥ �ð��� ��� ��)
	CTileMapComponent* pMapComponent = m_pRandomMap->CreateSceneComponent<CTileMapComponent>("RandomMap");
	m_pRandomMap->m_MapComponent = pMapComponent;
	m_pRandomMap->SetRootComponent(pMapComponent);

	// Tile���� �����Ѵ�.
	pMapComponent->CreateTile<CTile>(Tile_Shape::Rect, m_pRandomMap->m_MapSizeX, m_pRandomMap->m_MapSizeY, Vector2(TILE_SIZE_SMALL, TILE_SIZE_SMALL));

	//CTileFinder* pTileFinder(this);
	//m_pTileFinder = pTileFinder

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


	// ������� ������ Ÿ�� ������ ������� 
	// �Ļ��� �����Ѵ�.
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

}

void CMapGenerator::GenerateSea()
{
	// �� �ɼ��� ���߿� �� ���� �ɼ����� ����
}

void CMapGenerator::GenerateCoast()
{
	// ������ ���� ������� �ؾȰ� ����

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

		//Vector2 TileIndex = RandomIndex[RandomSeed];
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
	for (int i = 0; i < 5; i++)
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
	
}

void CMapGenerator::ChangeTileState(Vector2 tileIndex, TILE_STATE tileState)
{
	m_TileData[(int)tileIndex.x][(int)tileIndex.y] = tileState;

	CTile* pTile = m_pRandomMap->m_MapComponent->GetTile(tileIndex * TILE_SIZE_SMALL);
	pTile->SetFrameStart(tileState * TILE_SIZE_SMALL, 0.f);
	pTile->SetFrameEnd((tileState + 1) * TILE_SIZE_SMALL, TILE_SIZE_SMALL);

	// Ÿ���� ���°� ���� �Ǿ��� �� ������ �����Ѵ�
	ChangeTileState(tileIndex, tileState);
}

void CMapGenerator::ChangeTileStateData(Vector2 tileIndex, TILE_STATE tileState)
{
	if (tileIndex.x >= 0 && tileIndex.x < m_MapSizeX &&
		tileIndex.y >= 0 && tileIndex.y < m_MapSizeY)
	{
		return;
	}

	// �����Ͱ� �ִ��� ������ Ȯ�� ��, �̹� �ִ� �����Ͷ�� �ٲ� ������ �����Ѵ�.
}
