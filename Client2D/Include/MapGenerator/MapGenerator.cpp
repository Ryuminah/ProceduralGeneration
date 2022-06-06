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
					//m_AllTileStateData[TILE_STATE::LAND].push_back(Vector2(x, y));

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
	if (!m_pTileFinder->Check_ExistTile(tileIndex))
	{
		return;
	}

	CTile* pTile = m_pRandomMap->m_MapComponent->GetTile(tileIndex * TILE_SIZE_SMALL);
	pTile->SetFrameStart(tileState * TILE_SIZE_SMALL, 0.f);
	pTile->SetFrameEnd((tileState + 1) * TILE_SIZE_SMALL, TILE_SIZE_SMALL);

	// Ÿ���� ���°� Ȯ���� ���� �Ǿ��� �� ������ �����Ѵ�
	ChangeTileState(tileIndex, tileState);
}

void CMapGenerator::ChangeTileStateData(Vector2 tileIndex, TILE_STATE tileState)
{
	// �����Ͱ� �ִ��� ������ Ȯ�� ��, �̹� �ִ� �����Ͷ�� �ٲ� ������ �����Ѵ�.
	// ������ �߰�...

	m_TileData[(int)tileIndex.x][(int)tileIndex.y] = tileState;
}

void CMapGenerator::CreateRandom()
{
}
