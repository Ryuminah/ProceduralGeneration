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
#include "IMGUIManager.h"
#include "../GenerateWindow.h"

CMapGenerator::CMapGenerator() :
	m_IsGenerateWorldEnd(false),
	m_pRandomMap(nullptr),
	m_pTileFinder(nullptr),
	m_MapSizeX(50), m_MapSizeY(50),
	m_OutLineX(3), m_OutLineY(3),
	m_ForestMinX(7), m_ForestMinY(8), m_ForestMaxX(15), m_ForestMaxY(16),
	m_LakeMinX(5), m_LakeMinY(3), m_LakeMaxX(8), m_LakeMaxY(5)

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

	CTileMapComponent* pMapComponent = m_pRandomMap->CreateSceneComponent<CTileMapComponent>("RandomMap");
	m_pRandomMap->m_MapComponent = pMapComponent;
	m_pRandomMap->SetRootComponent(pMapComponent);

	// RandomMap�� MapSize ������ ������.
	//m_pRandomMap->m_MapSizeX;
	//m_pRandomMap->m_MapSizeY;


	// Tile���� �����Ѵ�.
	pMapComponent->CreateTile<CTile>(Tile_Shape::Rect, m_MapSizeX, m_MapSizeY, Vector2(TILE_SIZE_SMALL, TILE_SIZE_SMALL));
	m_pTileFinder = new CTileFinder(this);

	m_GenerateWindow = (CGenerateWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("GenerateWindow");
	m_GenerateWindow->SetGenerateOwner(this);
	m_GenerateWindow->Open();

	m_CurrentTileState = TILE_STATE::CLEAR;
	m_MapbaseShape = MAPBASE_SHAPE::DEFAULT;
	ResetWorld();

	return true;
}

bool CMapGenerator::Start()
{
	//m_RandomMap�� Start���� ȣ��
	GenerateWorld(TILE_STATE::CLEAR);

	return true;
}

void CMapGenerator::GenerateWorld(TILE_STATE _tileState)
{
	if (m_IsGenerateWorldEnd)
	{
		return;
	}

	m_CurrentTileState = _tileState;

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
}

void CMapGenerator::GenerateWorld()
{
	GenerateWorld(m_CurrentTileState);
}

// Member Function
void CMapGenerator::GenerateBase()
{
	switch (m_MapbaseShape)
	{
	case MAPBASE_SHAPE::DEFAULT:
		CreateDefaultMap();
		break;

	case MAPBASE_SHAPE::WIDTH:
		CreateWidthMap();
		break;

	case MAPBASE_SHAPE::HEIGHT:
		CreateHeightMap();
		break;

	default:
		break;
	}
}

void CMapGenerator::GenerateLand()
{
	// �̰͵� ������ ���� ���ϱ�

	// 1. ȣ�� ���� ���� ������ �޿�
	// �ɼ��� ���� ��� ������. . . �߰� .. .

	// ��Ÿ���� �ֺ� Ÿ���� ���̸� ������ ä���..
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
					ChangeTileStateImage(Vector2(x, y), TILE_STATE::LAND);
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

		ChangeTileStateImage(CoastTile, TILE_STATE::COAST);
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
				m_TileData[nearX][currentSandTile.y] != TILE_STATE::COAST)
			{
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
				m_TileData[currentSandTile.x][nearY] != TILE_STATE::COAST)
			{
				mapRandomSandTile.insert(std::pair<int, Vector2>(keyIndex, Vector2(currentSandTile.x, nearY)));
				++keyIndex;
			}
		}
	}

	//  Sand Tile �����
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

}

void CMapGenerator::GenerateLake()
{
	// �� ������ �����ǹǷ�, ���� �������� �ʴ´ٸ� �Լ� ����
	int ForestTilesCount = m_TileStateData[TILE_STATE::FOREST].size();

	if (ForestTilesCount == 0)
	{
		return;
	}

	int Index = 0;
	int randomCount = 2;

	int LakeX = (rand() % m_LakeMaxX) + m_LakeMinX;
	int LakeY = (rand() % m_LakeMaxY) + m_LakeMinY;

	// �� ���������� �ε����� �������� ��
	int startIndex = rand() % ForestTilesCount;
	Vector2 LakeStart = m_TileStateData[TILE_STATE::FOREST][startIndex];

	// ���� ������ ����
	Vector2 LakeEnd;
	LakeEnd.x = LakeStart.x + LakeX;
	LakeEnd.y = LakeStart.y + LakeY;

	// �簢���� �� �����
	std::vector<Vector2> vecLakeTile;
	for (int x = LakeStart.x; x < LakeEnd.x; ++x)
	{
		for (int y = LakeStart.y; y < LakeEnd.y; ++y)
		{
			// ȣ���� �� �������� �����Ѵ�
			if (m_pTileFinder->IsExistTile(x, y) &&
				m_pTileFinder->Check_TileState(x, y, TILE_STATE::FOREST))
			{
				ChangeTileStateImage(Vector2(x, y), TILE_STATE::LAKE);
				vecLakeTile.push_back(Vector2(x, y));
			}
		}
	}
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
			if (m_pTileFinder->IsExistTile(x,y) &&
				m_pTileFinder->Check_TileState(x,y,TILE_STATE::LAND))
			{
				ChangeTileStateImage(Vector2(x, y), TILE_STATE::FOREST);
				vecForestTile.push_back(Vector2(x, y));
			}
		}
	}

	// ��� �߰��� ���� ���ؼ��� �ܰ��� �����´�.
	std::vector<Vector2> vecSmoothTile = m_pTileFinder->Get_AreaBorder(vecForestTile, TILE_STATE::FOREST);
	std::vector<Vector2> vecOutlineTile = m_pTileFinder->Get_OutlineTiles(vecForestTile);

	
	for (size_t i = 0; i < vecOutlineTile.size(); ++i)
	{
		// �׵θ� �� �� Ÿ���� �ƴ϶�� ����� �ƿ��������� �����ϰ� ������ �������� �߰��Ѵ�.
		if (m_pTileFinder->Check_TileState(vecOutlineTile[i], TILE_STATE::LAND))
		{
			vecSmoothTile.push_back(vecOutlineTile[i]);
		}
	}

	for (size_t i = 0; i < vecSmoothTile.size(); ++i)
	{
		bool changeForest = rand() % 2;

		if (!changeForest)
		{
			ChangeTileStateImage(vecSmoothTile[i], TILE_STATE::LAND);
		}

		else
		{
			ChangeTileStateImage(vecSmoothTile[i], TILE_STATE::FOREST);
		}
	}


}

void CMapGenerator::ClearAll()
{
	m_TileData.clear();
	ResetWorld();
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
		ChangeTileStateImage(TileIndex, TILE_STATE::SEA);

		// ���� �ε����� value�� End�� ��ü, ���� ������ �� ����
		auto iterEnd = MapIndex.end();
		MapIndex[RandomSeed] = (--iterEnd)->second;
		MapIndex.erase(iterEnd);

		--RandomTileCount;
	}

	// ���� �����ؼ� �ε����� �����ѵ�, ���� ������ �����Ѵ�. (ũ�Ⱑ Ŀ������ ���귮�� �������� ���� ����.)

	// Smooth Map
	// ������ ������ ���� �� �ְ� �Ѵ�. Min / Normal /Max
	// 1. �ִ� ���������� Ƚ��(���̻� �ʿ� ��ȭ�� ����������) �� üũ�� �� Ƚ�� 
	// 2. �� ������ Ÿ�� ���¸� ���� ������ �� �߰��߰� Ȯ�� �Ǳ� ������ ���� ���¸� �� �� �ֵ��� �Ѵ� .. .. . . .
	//
	for (size_t i = 0; i < 4; i++)
	{
		SmoothMap();
	}
}

void CMapGenerator::UpgradeCellularAutomata()
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
	int TileHalfCount = RandomTileCount * 0.85f;
	int RandomSeed = 0;

	while (RandomTileCount)
	{
		// seed���� �̿��Ͽ� Ÿ���� ������ ��ġ�� ����
		// ���� Ÿ���� ���� �� �ε��� ����

		// ���� �̻��� ���� ��÷Ȯ���� ����
		if (RandomTileCount >= TileHalfCount)
		{
			int offset = MapIndex.size() * 0.6f;
			std::uniform_int_distribution<int> dist(offset, MapIndex.size());
			RandomSeed = dist(randomDevice);
		}

		else
		{
			std::uniform_int_distribution<int> dist(0, MapIndex.size());
			RandomSeed = dist(randomDevice);
		}

		Vector2 TileIndex = MapIndex[RandomSeed];

		// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (���� ����)
		ChangeTileStateImage(TileIndex, TILE_STATE::SEA);

		// ���� �ε����� value�� End�� ��ü, ���� ������ �� ����
		auto iterEnd = MapIndex.end();
		MapIndex[RandomSeed] = (--iterEnd)->second;
		MapIndex.erase(iterEnd);

		--RandomTileCount;
	}


	for (size_t i = 0; i < 5; ++i)
	{
		SmoothMap();
	}
}

void CMapGenerator::CreateDefaultMap()
{
	// �⺻ �� ���� ����
	// �õ尪�� �̿��ؼ� ���� ����� ����

	CEngine::GetInst()->OnDebugLog();
	clock_t start = clock(); // ���� �� �� ����

	// �� �ڵ�ȭ �˰���
	CellularAutomata();
	//UpgradeCellularAutomata();

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

			ChangeTileStateImage(LineStart, TILE_STATE::SEA);
			ChangeTileStateImage(LineEnd, TILE_STATE::SEA);
		}
	}

	for (int y = 0; y < m_OutLineY; ++y)
	{
		for (int x = 0; x < m_MapSizeX; ++x)
		{
			Vector2 LineStart = Vector2(x, y);
			Vector2 LineEnd = Vector2(x, m_MapSizeY - 1 - y);

			ChangeTileStateImage(LineStart, TILE_STATE::SEA);
			ChangeTileStateImage(LineEnd, TILE_STATE::SEA);
		}
	}
}

void CMapGenerator::CreateWidthMap()
{
	// �⺻ �� ���� ����
	// �õ尪�� �̿��ؼ� ���� ����� ����

	CEngine::GetInst()->OnDebugLog();
	clock_t start = clock(); // ���� �� �� ����

	// �� ũ�⸸ŭ ����Ʈ�� �ʿ� �����س���
	// ��÷ ��ȣ�� ��带 ����
	std::random_device randomDevice;
	std::mt19937_64 gen(randomDevice());

	std::unordered_map<int, Vector2> MapIndex;
	std::set<int> MapIndexKey;				// Ű�� �����س���
	std::vector<Vector2> RandomIndex;

	int Index = 0;
	int offsetY = (m_MapSizeY * 0.4f) * 0.5f;

	for (int x = 0; x < m_MapSizeX; ++x)
	{
		for (int y = 0; y < offsetY; ++y)
		{
			Vector2 index = Vector2((float)x, (float)y);
			Vector2 indexBottom = Vector2((float)x, m_MapSizeY - y );

			ChangeTileStateImage(index, TILE_STATE::SEA);
			ChangeTileStateImage(indexBottom, TILE_STATE::SEA);
		}
	}

	for (int x = 0; x < m_MapSizeX; ++x)
	{
		for (int y = offsetY; y < m_MapSizeY -offsetY ; ++y)
		{
			MapIndex[Index] = Vector2((float)x, (float)y);
			++Index;
		}
	}
	 
	int RandomTileCount = (m_MapSizeX * (m_MapSizeY - offsetY)) * 0.4f;
	int TileHalfCount = RandomTileCount * 0.7f;
	int RandomSeed = 0;

	while (RandomTileCount)
	{
		// seed���� �̿��Ͽ� Ÿ���� ������ ��ġ�� ����
		// ���� Ÿ���� ���� �� �ε��� ����

		// ���� �̻��� ���� ��÷Ȯ���� ����
		if (RandomTileCount >= TileHalfCount)
		{
			int offset = MapIndex.size() * 0.5f;
			std::uniform_int_distribution<int> dist(offset, MapIndex.size());
			RandomSeed = dist(randomDevice);
		}

		else
		{
			std::uniform_int_distribution<int> dist(0, MapIndex.size());
			RandomSeed = dist(randomDevice);
		}

		Vector2 TileIndex = MapIndex[RandomSeed];

		// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (���� ����)
		ChangeTileStateImage(TileIndex, TILE_STATE::SEA);

		// ���� �ε����� value�� End�� ��ü, ���� ������ �� ����
		auto iterEnd = MapIndex.end();
		MapIndex[RandomSeed] = (--iterEnd)->second;
		MapIndex.erase(iterEnd);

		--RandomTileCount;
	}

	for (size_t i = 0; i < 5; ++i)
	{
		SmoothMap();
	}

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

			ChangeTileStateImage(LineStart, TILE_STATE::SEA);
			ChangeTileStateImage(LineEnd, TILE_STATE::SEA);
		}
	}

	//for (int y = 0; y < m_OutLineY; ++y)
	//{
	//	for (int x = 0; x < m_MapSizeX; ++x)
	//	{
	//		Vector2 LineStart = Vector2(x, y);
	//		Vector2 LineEnd = Vector2(x, m_MapSizeY - 1 - y);

	//		ChangeTileStateImage(LineStart, TILE_STATE::SEA);
	//		ChangeTileStateImage(LineEnd, TILE_STATE::SEA);
	//	}
	//}
}

void CMapGenerator::CreateHeightMap()
{
	// �⺻ �� ���� ����
	// �õ尪�� �̿��ؼ� ���� ����� ����

	CEngine::GetInst()->OnDebugLog();
	clock_t start = clock(); // ���� �� �� ����

	// �� ũ�⸸ŭ ����Ʈ�� �ʿ� �����س���
	// ��÷ ��ȣ�� ��带 ����
	std::random_device randomDevice;
	std::mt19937_64 gen(randomDevice());

	std::unordered_map<int, Vector2> MapIndex;
	std::set<int> MapIndexKey;				// Ű�� �����س���
	std::vector<Vector2> RandomIndex;

	int Index = 0;
	int offsetX = (m_MapSizeX * 0.4f) * 0.5f;

	for (int x = 0; x < offsetX; ++x)
	{
		for (int y = 0; y < m_MapSizeY; ++y)
		{
			Vector2 index = Vector2((float)x, (float)y);
			Vector2 indexBottom = Vector2((float)m_MapSizeX - x,y);

			ChangeTileStateImage(index, TILE_STATE::SEA);
			ChangeTileStateImage(indexBottom, TILE_STATE::SEA);
		}
	}

	for (int y = 0; y < m_MapSizeY; ++y)
	{
		for (int x = offsetX; x < m_MapSizeX - offsetX; ++x)
		{
			MapIndex[Index] = Vector2((float)x, (float)y);
			++Index;
		}
	}

	int RandomTileCount = ((m_MapSizeX - offsetX) * m_MapSizeY) * 0.4f;

	// ���ϴ� ���� Ŀ�� ���� ���� ���� Ÿ���� ���� ������Ų��
	int TileHalfCount = RandomTileCount * 0.8f;
	int RandomSeed = 0;

	while (RandomTileCount)
	{
		// ���� �̻��� ���� ��÷Ȯ���� ����
		std::uniform_int_distribution<int> dist(0, MapIndex.size());
		RandomSeed = dist(randomDevice);

		if (RandomTileCount >= TileHalfCount)
		{
			int offset = MapIndex.size() * 0.5f;
			std::uniform_int_distribution<int> dist(offset, MapIndex.size() );
			RandomSeed = dist(randomDevice);
		}

		else
		{
			std::uniform_int_distribution<int> dist(0, MapIndex.size());
			RandomSeed = dist(randomDevice);
		}

		Vector2 TileIndex = MapIndex[RandomSeed];

		// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (���� ����)
		ChangeTileStateImage(TileIndex, TILE_STATE::SEA);

		// ���� �ε����� value�� End�� ��ü, ���� ������ �� ����
		auto iterEnd = MapIndex.end();
		MapIndex[RandomSeed] = (--iterEnd)->second;
		MapIndex.erase(iterEnd);

		--RandomTileCount;
	}

	for (size_t i = 0; i < 5; ++i)
	{
		SmoothMap();
	}

	clock_t end = clock();
	double clockTime = (double)(end - start) / CLOCKS_PER_SEC;
	char buffer[BUFSIZ];
	sprintf_s(buffer, "%f", clockTime);
	CEngine::GetInst()->AddDebugLog(buffer);

	for (int y = 0; y < m_OutLineY; ++y)
	{
		for (int x = 0; x < m_MapSizeX; ++x)
		{
			Vector2 LineStart = Vector2(x, y);
			Vector2 LineEnd = Vector2(x, m_MapSizeY - 1 - y);

			ChangeTileStateImage(LineStart, TILE_STATE::SEA);
			ChangeTileStateImage(LineEnd, TILE_STATE::SEA);
		}
	}
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
				ChangeTileStateImage(Vector2(x, y), TILE_STATE::SEA);
			}

			else if (NearSeaCount < 4)
			{
				// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (������ ����)
				ChangeTileStateImage(Vector2(x, y), TILE_STATE::LAND);
			}
		}
	}
}


void CMapGenerator::ChangeTileStateImage(Vector2 tileIndex, TILE_STATE tileState)
{
	if (!m_pTileFinder->IsExistTile(tileIndex))
	{
		return;
	}

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

void CMapGenerator::DebugFunctionTime(void(CMapGenerator::*pFunc)())
{
	clock_t start = clock();

	//(*pFunc)();

	clock_t end = clock();
	double clockTime = (double)(end - start) / CLOCKS_PER_SEC;
	char buffer[BUFSIZ];
	sprintf_s(buffer, "%f", clockTime);
	CEngine::GetInst()->AddDebugLog(buffer);
}

void CMapGenerator::CreateRandom()
{

}

void CMapGenerator::ResetWorld()
{
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
			ChangeTileStateImage(Vector2(x, y), TILE_STATE::LAND);
		}
	}
}
