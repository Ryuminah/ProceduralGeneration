#include <time.h>
#include <random>
#include <set>
#include <map>
#include "MapGenerator.h"
#include "TileFinder.h"
#include "MapGeneratorManager.h"
#include "RandomMap.h"
#include "Scene/Scene.h"
#include "Resource/Material.h"
#include "Scene/SceneResource.h"
#include "Engine.h"
#include "Timer.h"
#include "Input.h"

CRandomMap::CRandomMap() : m_MapSizeX(50), m_MapSizeY(50),
	m_IsGenerateFinished(false)
{
}

CRandomMap::CRandomMap(const CRandomMap& obj) :
	CGameObject(obj)
{
	m_MapComponent = (CTileMapComponent*)FindSceneComponent("RandomMap");
}

CRandomMap::~CRandomMap()
{
}

void CRandomMap::Start()
{
	CGameObject::Start();

	GenerateMapBase();
}

bool CRandomMap::Init()
{
	CGameObject::Init();

	// Load RandomMap Texture 
	m_pScene->GetResource()->CreateMaterial("Tile_16px");
	m_pScene->GetResource()->AddMaterialTexture("Tile_16px", "Tile_16px",
		TEXT("RandomMap/Tile_16px.png"));
	m_pScene->GetResource()->SetMaterialTransparency("Tile_16px", true);
	m_pScene->GetResource()->SetMaterialShader("Tile_16px", "TileMapShader");

	m_pScene->GetResource()->CreateMaterial("Tile_Land");
	m_pScene->GetResource()->AddMaterialTexture("Tile_Land", "Tile_Land",
		TEXT("RandomMap/Tile_Land_16px.png"));
	m_pScene->GetResource()->SetMaterialTransparency("Tile_Land", true);
	m_pScene->GetResource()->SetMaterialShader("Tile_Land", "TileMapShader");
	
	m_pScene->GetResource()->CreateMaterial("Tile_Sea");
	m_pScene->GetResource()->AddMaterialTexture("Tile_Sea", "Tile_Sea",
		TEXT("RandomMap/Tile_Sea_16px.png"));
	m_pScene->GetResource()->SetMaterialTransparency("Tile_Sea", true);
	m_pScene->GetResource()->SetMaterialShader("Tile_Sea", "TileMapShader");
	
	m_MapGenerator = CMapGeneratorManager::GetInst()->CreateMapGenerator<CMapGenerator>("RandomMap", this);
	
	m_MapComponent->SetMaterial(0, "Tile_16px");
	m_MapComponent->SetFrameMax(6, 1);
	m_MapComponent->SetTileDefaultFrame(0, 0);
	m_MapComponent->SetPivot(0.f, 0.f, 0.f);
	m_MapComponent->SetRelativePos(0.f, 0.f, 0.1f);
	m_MapComponent->SetAnimation2DEnable(false);

	CInput::GetInst()->AddKeyCallback<CRandomMap>("Space", KT_Down, this, &CRandomMap::SmoothMap);
	CInput::GetInst()->AddKeyCallback<CRandomMap>("1", KT_Down, this, &CRandomMap::Clear);
	CInput::GetInst()->AddKeyCallback<CRandomMap>("2", KT_Down, this, &CRandomMap::SetOnlyLand);
	CInput::GetInst()->AddKeyCallback<CRandomMap>("3", KT_Down, this, &CRandomMap::CreateForest);

	CInput::GetInst()->AddKeyCallback<CRandomMap>("Enter", KT_Down, this, &CRandomMap::SetGenerateFinished);


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

void CRandomMap::GenerateMapBase()
{
	CEngine::GetInst()->OnDebugLog();
	m_IsGenerateFinished = false;

	for (int x = 0; x < m_MapSizeX; ++x)
	{
		std::vector<int> tileData;

		for (int y = 0; y < m_MapSizeY; ++y)
		{
			tileData.push_back(TILE_STATE::LAND);
		}

		m_TileData.push_back(tileData);
	}

	clock_t start = clock(); // ���� �ð� ����
	
	IndexRandomLogic();
	
	// �̰� ��¥ ������ �ȳ��±� ����
	//RandomSaveLogic();		


	clock_t end = clock(); 
	double clockTime = (double)(end - start) / CLOCKS_PER_SEC;
	char buffer[BUFSIZ];
	sprintf_s(buffer, "%f", clockTime);


	// �Ŀ� ���� ���� �����ڸ��� ���� �����Ѵ�
	for (int x = 0; x < m_MapSizeX; ++x)
	{
		ChangeTileImage(Vector2((float)x, 0.f), TILE_STATE::SEA);
		ChangeTileImage(Vector2((float)x, (float)(m_MapSizeY - 1)), TILE_STATE::SEA);

		ChangeTileImage(Vector2((float)x , 1.f), TILE_STATE::SEA);
		ChangeTileImage(Vector2((float)x , (float)(m_MapSizeY - 2)), TILE_STATE::SEA);
	}

	for (int y = 0; y < m_MapSizeY; ++y)
	{
		ChangeTileImage(Vector2(0.f, (float)y), TILE_STATE::SEA);
		ChangeTileImage(Vector2((float)(m_MapSizeX - 1), (float)y), TILE_STATE::SEA);

		ChangeTileImage(Vector2(1.f, (float)y), TILE_STATE::SEA);
		ChangeTileImage(Vector2((float)(m_MapSizeX - 2), (float)y), TILE_STATE::SEA);
	}

	// �ɸ� �ð� ���
	// ����: ��(second)
	// CLOCKS_PER_SEC�� ������� �ʴ����� ���ɴϴ�.

	CEngine::GetInst()->AddDebugLog(buffer);
}


void CRandomMap::GenerateEnvironment(float DeltaTime)
{
	// ��, �ٴ� ���� ���� ȯ�� ����

	// �� �����̸� �Ǵ��Ѵ�.
}

void CRandomMap::SetOnlyLand(float DeltaTime)
{
	// �ֺ� Ÿ�� 4���̻� ���̸� ������ �ٲ� (�ִ��� ������ �ٲ�)
	for (int x = 3; x < m_MapSizeX-3; ++x)
	{
		for (int y = 3; y < m_MapSizeY-3; ++y)
		{
			if (m_TileData[x][y] == TILE_STATE::SEA)
			{
				int NearSeaCount = CheckNearSeaTile8(x, y);

				if (NearSeaCount <= 4)
				{
					ChangeTileImage(Vector2(x,y), TILE_STATE::LAND);
					//m_AllTileStateData[TILE_STATE::LAND].push_back(Vector2(x, y));

				}
			}
		}
	}
}

void CRandomMap::SetGenerateFinished(float DeltaTime)
{
	// ������ �� ����� ������Ų �� ȯ���� �����Ѵ�
	std::vector<Vector2> vecSeaTile;
	std::vector<Vector2> vecLandTile;
	std::vector<Vector2> vecCoastTile;

	for (int x = 0; x < m_MapSizeX; ++x)
	{
		for (int y = 0; y < m_MapSizeY; ++y)
		{
			if (m_TileData[x][y] == TILE_STATE::SEA)
			{
				vecSeaTile.push_back(Vector2(x, y));
			}
			
			else
			{
				vecLandTile.push_back(Vector2(x, y));

			}
		}
	}

	//m_AllTileStateData.insert(std::pair<TILE_STATE, std::vector<Vector2>>(TILE_STATE::SEA, vecSeaTile));
	//m_AllTileStateData.insert(std::pair<TILE_STATE, std::vector<Vector2>>(TILE_STATE::LAND, vecLandTile));
	m_IsGenerateFinished = true;
	
	// Land �� ������ �ִ� Ÿ�� üũ �� �𷡷� �ѹ� �ٲ�.
	for (size_t i = 0; i < vecLandTile.size(); ++i)
	{
		Vector2 Index = vecLandTile[i];
		int NearSeaCount = CheckNearSeaTile4(Index.x, Index.y);

		if (NearSeaCount > 0)
		{
			ChangeTileImage(Index, TILE_STATE::COAST);

			//m_TileData[Index.x][Index.y] = TILE_STATE::COAST;

			vecCoastTile.push_back(Index);
		}
	}

	//m_AllTileStateData.insert(std::pair<TILE_STATE, std::vector<Vector2>>(TILE_STATE::COAST, vecCoastTile));
	int keyIndex = 0;
	std::map<int, Vector2> mapRandomSandTile;


	CreateCoast(vecCoastTile);

}

void CRandomMap::CreateCoast(std::vector<Vector2> vecCoastTile)
{
	int keyIndex = 0;
	std::map<int, Vector2> mapRandomSandTile;

	// ���� ����� �������� ...
	for (size_t i = 0; i < vecCoastTile.size(); ++i)
	{
		Vector2 currentSandTile = vecCoastTile[i];

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

		ChangeTileImage(TileIndex, TILE_STATE::COAST);
		//m_AllTileStateData.insert(std::pair<TILE_STATE, std::vector<Vector2>>(TILE_STATE::COAST, vecCoastTile));
		vecCoastTile.push_back(TileIndex);

		// ���� �ε����� value�� End�� ��ü, ���� ������ �� ����
		auto iterEnd = mapRandomSandTile.end();
		mapRandomSandTile[RandomSeed] = (--iterEnd)->second;
		mapRandomSandTile.erase(iterEnd);

		--RandomTileCount;
	}

	m_AllTileStateData.insert(std::pair<TILE_STATE, std::vector<Vector2>>(TILE_STATE::COAST, vecCoastTile));
}


void CRandomMap::IndexRandomLogic()
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
		//RandomSeed = rand() % RandomIndex.size();
		std::uniform_int_distribution<int> dist(0, MapIndex.size());
		RandomSeed = dist(randomDevice);

	 	//Vector2 TileIndex = RandomIndex[RandomSeed];
		Vector2 TileIndex = MapIndex[RandomSeed];

		// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (���� ����)
		ChangeTileImage(TileIndex, TILE_STATE::SEA);

		// ���� �ε����� value�� End�� ��ü, ���� ������ �� ����
		auto iterEnd = MapIndex.end();
		MapIndex[RandomSeed] = (--iterEnd)->second;
		MapIndex.erase(iterEnd);

		--RandomTileCount;
	}
}

void CRandomMap::RandomSaveLogic()
{
	// �� ũ�⸸ŭ ����Ʈ�� �ʿ� �����س���
	// ��÷ ��ȣ�� ��带 ����
	std::random_device randomDevice;
	std::mt19937_64 gen(randomDevice());
	std::unordered_map<int, Vector2> MapIndex;
	std::unordered_map<int, int> RandomIndex;
	std::unordered_map<int, Vector2> MapData;

	int RandomSeed = 0;
	int Index = 0;

	for (int x = 0; x < m_MapSizeX; ++x)
	{
		for (int y = 0; y < m_MapSizeY; ++y)
		{
			MapIndex[Index] = Vector2((float)x, (float)y);

			// ���� ����� Ű ���� second
			RandomIndex[Index] = Index;
			++Index;
		}
	}
	                              
	while (RandomIndex.size() != 0)
	{
		std::uniform_int_distribution<int> dist(0, RandomIndex.size());
		RandomSeed = dist(randomDevice);

		//Vector2 TileIndex = RandomIndex[RandomSeed];
		// ���� �ε����� �����س��� second�� ������ ��¥ TileIndex�� Ž��
		
		Vector2 TileIndex = MapIndex[RandomIndex[RandomSeed]];

		// �������� �������
		MapData[RandomIndex[RandomSeed]] = TileIndex;

		// �ε����� ����
		RandomIndex.erase(RandomSeed);
	}

	// 50���� ������ ���� ä��
	int RandomTileCount = (int)(m_MapSizeX * m_MapSizeY) * 0.42f;

	for (size_t i = 0; i < RandomTileCount; ++i)
	{
		auto iter = MapData.begin();
		auto iterEnd = MapData.end();

		ChangeTileImage(iter->second, TILE_STATE::SEA);

		++iter;
	}

	//while (RandomTileCount)
	//{
	//	// seed���� �̿��Ͽ� Ÿ���� ������ ��ġ�� ����
	//	// ���� Ÿ���� ���� �� �ε��� ����
	//	//RandomSeed = rand() % RandomIndex.size();
	//	RandomSeed = dist(randomDevice);

	//	//Vector2 TileIndex = RandomIndex[RandomSeed];
	//	Vector2 TileIndex = RandomIndex[RandomSeed];

	//	// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (���� ����)
	//	ChangeTileImage(TileIndex, LAND_STATE::SEA);

	//	// ���� �ε����� value�� End�� ��ü, ���� ������ �� ����
	//	auto iterEnd = RandomIndex.end();
	//	RandomIndex[RandomSeed] = (--iterEnd)->second;
	//	RandomIndex.erase(iterEnd);

	//	//RandomIndex.erase(RandomSeed);


	//	--RandomTileCount;
	//}
}


void CRandomMap::TestRandomLogic()
{
	// ������ ��ġ�� �� Ÿ���� �����Ѵ�.
	srand((unsigned int)time(NULL));

	// 50���� ������ ���� ä��
	int RandomTileCount = (m_MapSizeX * m_MapSizeY) * 0.51f;
	std::vector<Vector2> vecCreateIndex;
	Vector2 TileIndex = { 0.f,0.f };

	// ��� ���� 
	while (RandomTileCount)
	{
		// seed���� �̿��Ͽ� Ÿ���� ������ ��ġ�� ����
		TileIndex.x = rand() % m_MapSizeX;
		TileIndex.y = rand() % m_MapSizeY;

		if (vecCreateIndex.size() != 0)
		{
			// Vector�� ��ȸ�ϸ鼭 �̹� ����� �ε������ �ٽ� �������� ������
			for (int i = 0; i < vecCreateIndex.size(); ++i)
			{
				if ((vecCreateIndex[i].x == TileIndex.x) && (vecCreateIndex[i].x == TileIndex.y))
				{
					// �ٽ� �̰� �ٽ� ����
					TileIndex.x = (float)(rand() % m_MapSizeX);
					TileIndex.y = (float)(rand() % m_MapSizeY);

					i = 0;
				}
			}
		}

		// �̹� ������ �ε����� ����
		vecCreateIndex.push_back(TileIndex);

		// �ٴٷ� ����
		ChangeTileImage(TileIndex, TILE_STATE::SEA);

		--RandomTileCount;
	}
}

void CRandomMap::CreateForest(float DeltaTime)
{
	int Index = 0;

	int randomCount = 2;
	int forestX = (rand() % 15) + 7;
	int forestY = (rand() % 16) +8;

	
	Vector2 forestStart;
	int startIndex = rand() % m_AllTileStateData[TILE_STATE::LAND].size();
	forestStart = m_AllTileStateData[TILE_STATE::LAND][startIndex];


	Vector2 forestEnd;
	forestEnd.x = forestStart.x + forestX;
	forestEnd.y = forestStart.y + forestY;

	std::vector<Vector2> vecForestTile;
	for (int x = forestStart.x; x < forestEnd.x; ++x)
	{
		for (int y = forestStart.y; y < forestEnd.y; ++y)
		{
			if (x >= 0 && x < m_MapSizeX &&
				y >= 0 && y < m_MapSizeY &&
				m_TileData[x][y] == TILE_STATE::LAND)
			{
				vecForestTile.push_back(Vector2(x, y));
				ChangeTileImage(Vector2(x, y), TILE_STATE::FOREST);
			}

		}
	}

	m_AllTileStateData.insert(std::pair<TILE_STATE, std::vector<Vector2>>(TILE_STATE::FOREST, vecForestTile));
}

void CRandomMap::SmoothMap(float DeltaTime)
{
	if (m_IsGenerateFinished)
		return;

	for (int x = 0; x < m_MapSizeX; ++x)
	{
		for (int y = 0; y < m_MapSizeY; ++y)
		{
			int NearSeaCount = CheckNearSeaTile8(x, y);

			if (NearSeaCount > 4)
			{
				// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (���� ����)
				ChangeTileImage(Vector2(x, y), TILE_STATE::SEA);
			}

			else if (NearSeaCount < 4)
			{
				// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (������ ����)
				ChangeTileImage(Vector2(x, y), TILE_STATE::LAND);
			}
		}
	}
}

void CRandomMap::Clear(float DeltaTime)
{
	m_TileData.clear();

	for (int x = 0; x < m_MapSizeX; ++x)
	{
		for (int y = 0; y < m_MapSizeY; ++y)
		{
			// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (������ ����)
			CTile* pTile = m_MapComponent->GetTile(Vector2((float)x, (float)y) * TILE_SIZE_SMALL);
			pTile->SetFrameStart(0.f, 0.f);
			pTile->SetFrameEnd(TILE_SIZE_SMALL, TILE_SIZE_SMALL);
		}
	}

	GenerateMapBase();
}


void CRandomMap::PickRandom()
{

}

bool CRandomMap::ChangeTileImage(Vector2 tileIndex, TILE_STATE tileState)
{
	bool result = false;

	CTile* pTile = m_MapComponent->GetTile(tileIndex * TILE_SIZE_SMALL);
	if (pTile)
	{
		pTile->SetFrameStart(tileState * TILE_SIZE_SMALL, 0.f);
		pTile->SetFrameEnd((tileState + 1) * TILE_SIZE_SMALL, TILE_SIZE_SMALL);

		m_TileData[tileIndex.x][tileIndex.y] = tileState;
		m_AllTileStateData[tileState].push_back(tileIndex);

		result = true;
	}

	return result;
}


int CRandomMap::CheckNearSeaTile8(int indexX, int indexY)
{
	int wallCount = 0;

	for (int nearX = indexX - 1; nearX <= indexX + 1; nearX++)
	{
		for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
		{
			// �� ���� üũ
			if (nearX >= 0 && nearX < m_MapSizeX && nearY >= 0 && nearY < m_MapSizeY)
			{
				if (nearX != m_MapSizeX || nearY != m_MapSizeY)
				{
					wallCount += m_TileData[nearX][nearY];
				}
			}

			else
			{
				wallCount++;
			}
		}
	}

	return wallCount;
}

int CRandomMap::CheckNearSeaTile4(int indexX, int indexY)
{
	int wallCount = 0;

	for (int nearX = indexX - 1; nearX <= indexX + 1; ++nearX)
	{
		if (nearX == indexX)
		{
			continue;
		}

		// �� ���� �̳����
		if ((nearX >= 0 && nearX < m_MapSizeX)&&
			m_TileData[nearX][indexY] == TILE_STATE::SEA)
		{
			++wallCount;
		}
		
	}

	for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
	{
		if (nearY == indexY)
		{
			continue;
		}

		// �� ���� üũ
		if ((nearY >= 0 && nearY < m_MapSizeY) &&
			m_TileData[indexX][nearY] == TILE_STATE::SEA)
		{
			++wallCount;
		}
	}

	return wallCount;
}

//bool CRandomMap::CheckNearTileState4(Vector2 index, TILE_STATE checkTileState)
//{
//	bool result = false;
//
//	if (m_TileData[index.x][index.y] == checkTileState)
//	{
//		result = true;
//	}
//
//	return result;
//}

int CRandomMap::CheckNearTileState4(int indexX, int indexY, TILE_STATE checkTileState)
{
	int tileCount = 0;

	for (int nearX = indexX - 1; nearX <= indexX + 1; ++nearX)
	{
		if (nearX == indexX)
		{
			continue;
		}

		// �� ���� �̳����
		if ((nearX >= 0 && nearX < m_MapSizeX) &&
			m_TileData[nearX][indexY] == checkTileState)
		{
			++tileCount;
		}
	}

	for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
	{
		if (nearY == indexY)
		{
			continue;
		}

		// �� ���� üũ
		if ((nearY >= 0 && nearY < m_MapSizeY) &&
			m_TileData[indexX][nearY] == checkTileState)
		{
			++tileCount;
		}
	}

	return tileCount;
}

int CRandomMap::CheckNearTileState4(Vector2 index, TILE_STATE checkTileState)
{
	return CheckNearTileState4((int)index.x ,(int)index.y ,checkTileState);
}



int CRandomMap::CheckNearTileState(int indexX, int indexY, TILE_STATE checkTileState)
{
	int tileCount = 0;

	for (int nearX = indexX - 1; nearX <= indexX + 1; ++nearX)
	{
		if (nearX == indexX)
		{
			continue;
		}

		// �� ���� �̳����
		if ((nearX >= 0 && nearX < m_MapSizeX) &&
			m_TileData[nearX][indexY] == checkTileState)
		{
			++tileCount;
		}
	}

	for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
	{
		if (nearY == indexY)
		{
			continue;
		}

		// �� ���� üũ
		if ((nearY >= 0 && nearY < m_MapSizeY) &&
			m_TileData[indexX][nearY] == checkTileState)
		{
			++tileCount;
		}
	}

	return tileCount;
}

