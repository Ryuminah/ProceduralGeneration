#include <time.h>
#include "../MapGenerator/MapGenerator.h"
#include <random>
#include "RandomMap.h"
#include "Scene/Scene.h"
#include "Resource/Material.h"
#include "Scene/SceneResource.h"
#include "Engine.h"
#include "Timer.h"
#include "Input.h"
#include <set>
#include <map>



CRandomMap::CRandomMap() : m_MapX(50), m_MapY(50),
	m_IsGenerateFinished(false)
{
}

CRandomMap::CRandomMap(const CRandomMap& obj) :
	CGameObject(obj)
{
	m_RandomMap = (CTileMapComponent*)FindSceneComponent("RandomMap");
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

	m_RandomMap = CreateSceneComponent<CTileMapComponent>("RandomMap");

	SetRootComponent(m_RandomMap);
	m_RandomMap->CreateTile<CTile>(Tile_Shape::Rect,m_MapX,m_MapY, Vector2(TILE_SIZE_SMALL, TILE_SIZE_SMALL));
	m_RandomMap->SetMaterial(0, "Tile_16px");
	m_RandomMap->SetFrameMax(4, 1);
	m_RandomMap->SetTileDefaultFrame(0, 0);
	m_RandomMap->SetPivot(0.f, 0.f, 0.f);
	m_RandomMap->SetRelativePos(0.f, 0.f, 0.1f);
	m_RandomMap->SetAnimation2DEnable(false);

	CInput::GetInst()->AddKeyCallback<CRandomMap>("Space", KT_Down, this, &CRandomMap::SmoothMap);
	CInput::GetInst()->AddKeyCallback<CRandomMap>("1", KT_Down, this, &CRandomMap::Clear);
	CInput::GetInst()->AddKeyCallback<CRandomMap>("2", KT_Down, this, &CRandomMap::SetOnlyLand);
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

	for (int x = 0; x < m_MapX; ++x)
	{
		std::vector<int> tileData;

		for (int y = 0; y < m_MapY; ++y)
		{
			tileData.push_back(LAND_STATE::LAND);
		}

		m_TileData.push_back(tileData);
	}

	clock_t start = clock(); // ���� �ð� ����
	
	IndexRandomLogic();
	//TestRandomLogic();
	
	// �̰� ��¥ ������ �ȳ��±� ����
	//RandomSaveLogic();		


	clock_t end = clock(); 
	double clockTime = (double)(end - start) / CLOCKS_PER_SEC;
	char buffer[BUFSIZ];
	sprintf_s(buffer, "%f", clockTime);


	// �Ŀ� ���� ���� �����ڸ��� ���� �����Ѵ�
	for (int x = 0; x < m_MapX; ++x)
	{
		ChangeTileImage(Vector2((float)x, 0.f), LAND_STATE::SEA);
		ChangeTileImage(Vector2((float)x, (float)(m_MapY - 1)), LAND_STATE::SEA);

		ChangeTileImage(Vector2((float)x , 1.f), LAND_STATE::SEA);
		ChangeTileImage(Vector2((float)x , (float)(m_MapY - 2)), LAND_STATE::SEA);
	}

	for (int y = 0; y < m_MapY; ++y)
	{
		ChangeTileImage(Vector2(0.f, (float)y), LAND_STATE::SEA);
		ChangeTileImage(Vector2((float)(m_MapX - 1), (float)y), LAND_STATE::SEA);

		ChangeTileImage(Vector2(1.f, (float)y), LAND_STATE::SEA);
		ChangeTileImage(Vector2((float)(m_MapX - 2), (float)y), LAND_STATE::SEA);
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
	for (int x = 3; x < m_MapX-3; ++x)
	{
		for (int y = 3; y < m_MapY-3; ++y)
		{
			if (m_TileData[x][y] == LAND_STATE::SEA)
			{
				int NearSeaCount = CheckNearSeaTile8(x, y);

				if (NearSeaCount <= 4)
				{
					ChangeTileImage(Vector2(x,y), LAND_STATE::LAND);
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

	for (int x = 0; x < m_MapX; ++x)
	{
		for (int y = 0; y < m_MapY; ++y)
		{
			if (m_TileData[x][y] == LAND_STATE::SEA)
			{
				vecSeaTile.push_back(Vector2(x, y));
			}
			
			else
			{
				vecLandTile.push_back(Vector2(x, y));

			}
		}
	}

	m_AllTileStateData.insert(std::pair<LAND_STATE, std::vector<Vector2>>(LAND_STATE::SEA, vecSeaTile));
	m_AllTileStateData.insert(std::pair<LAND_STATE, std::vector<Vector2>>(LAND_STATE::LAND, vecLandTile));

	m_IsGenerateFinished = true;
	
	// Land �� ������ �ִ� Ÿ�� üũ �� �𷡷� �ѹ� �ٲ�.
	for (size_t i = 0; i < vecLandTile.size(); ++i)
	{
		Vector2 Index = vecLandTile[i];
		int NearSeaCount = CheckNearSeaTile4(Index.x, Index.y);

		if (NearSeaCount > 0)
		{
			ChangeTileImage(Index, LAND_STATE::COAST);
			m_TileData[Index.x][Index.y] = LAND_STATE::COAST;
			vecCoastTile.push_back(Index);
		}
	}

	m_AllTileStateData.insert(std::pair<LAND_STATE, std::vector<Vector2>>(LAND_STATE::COAST, vecCoastTile));
	int keyIndex = 0;
	std::map<int, Vector2> mapRandomSandTile;

	CreateCoast(vecCoastTile);

	//keyIndex = 0;
	//mapRandomSandTile.clear();

	//// �ѹ� �� �ݺ� (�̰� �߸��ڵ� �����ϱ�..)
	//for (size_t i = 0; i < vecCoastTile.size(); ++i)
	//{
	//	Vector2 currentSandTile = vecCoastTile[i];

	//	for (int nearX = currentSandTile.x - 1; nearX <= currentSandTile.x + 1; ++nearX)
	//	{
	//		if (nearX == currentSandTile.x)
	//		{
	//			continue;
	//		}

	//		// �� ���� �̳��� Ÿ���� �𷡰� �ƴ϶�� �����Ѵ�
	//		if ((nearX >= 0 && nearX < m_MapX) &&
	//			m_TileData[nearX][currentSandTile.y])
	//		{
	//			m_TileData[nearX][currentSandTile.y] != LAND_STATE::COAST;
	//			mapRandomSandTile.insert(std::pair<int, Vector2>(keyIndex, Vector2(nearX, currentSandTile.y)));

	//			++keyIndex;
	//		}
	//	}

	//	for (int nearY = currentSandTile.y - 1; nearY <= currentSandTile.y + 1; nearY++)
	//	{
	//		if (nearY == currentSandTile.y)
	//		{
	//			continue;
	//		}

	//		// �� ���� üũ
	//		if ((nearY >= 0 && nearY < m_MapY) &&
	//			m_TileData[currentSandTile.x][nearY])
	//		{
	//			m_TileData[currentSandTile.x][nearY] != LAND_STATE::COAST;
	//			mapRandomSandTile.insert(std::pair<int, Vector2>(keyIndex, Vector2(currentSandTile.x, nearY)));
	//			++keyIndex;
	//		}
	//	}
	//}

	//// �������� �̾Ƽ� . .. . . ���� Ÿ�� �����

	//RandomTileCount = mapRandomSandTile.size() * 0.5f;
	//RandomSeed = 0;

	//while (RandomTileCount)
	//{
	//	// seed���� �̿��Ͽ� Ÿ���� ������ ��ġ�� ����
	//	// ���� Ÿ���� ���� �� �ε��� ����
	//	std::uniform_int_distribution<int> dist(0, mapRandomSandTile.size());
	//	RandomSeed = dist(randomDevice);

	//	Vector2 TileIndex = mapRandomSandTile[RandomSeed];

	//	// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (���� ����)
	//	ChangeTileImage(TileIndex, LAND_STATE::COAST);

	//	// ���� �ε����� value�� End�� ��ü, ���� ������ �� ����
	//	auto iterEnd = mapRandomSandTile.end();
	//	mapRandomSandTile[RandomSeed] = (--iterEnd)->second;
	//	mapRandomSandTile.erase(iterEnd);

	//	--RandomTileCount;
	//}

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
			if ((nearX >= 0 && nearX < m_MapX) &&
				m_TileData[nearX][currentSandTile.y])
			{
				m_TileData[nearX][currentSandTile.y] != LAND_STATE::COAST;
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
			if ((nearY >= 0 && nearY < m_MapY) &&
				m_TileData[currentSandTile.x][nearY])
			{
				m_TileData[currentSandTile.x][nearY] != LAND_STATE::COAST;
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

		// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (���� ����)
		ChangeTileImage(TileIndex, LAND_STATE::COAST);
		m_AllTileStateData.insert(std::pair<LAND_STATE, std::vector<Vector2>>(LAND_STATE::COAST, vecCoastTile));
		vecCoastTile.push_back(TileIndex);

		// ���� �ε����� value�� End�� ��ü, ���� ������ �� ����
		auto iterEnd = mapRandomSandTile.end();
		mapRandomSandTile[RandomSeed] = (--iterEnd)->second;
		mapRandomSandTile.erase(iterEnd);

		--RandomTileCount;
	}

}


void CRandomMap::IndexRandomLogic()
{
	// �� ũ�⸸ŭ ����Ʈ�� �ʿ� �����س���
	// ��÷ ��ȣ�� ��带 ����
	std::random_device randomDevice;
	std::mt19937_64 gen(randomDevice());
	std::unordered_map<int, Vector2> MapIndex;
	int Index = 0;

	for (int x = 0; x < m_MapX; ++x)
	{
		for (int y = 0; y < m_MapY; ++y)
		{
			MapIndex[Index] = Vector2((float)x, (float)y);
			++Index;
		}
	}

	int RandomTileCount = (m_MapX * m_MapY) * 0.42f;	
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
		ChangeTileImage(TileIndex, LAND_STATE::SEA);

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

	for (int x = 0; x < m_MapX; ++x)
	{
		for (int y = 0; y < m_MapY; ++y)
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
	int RandomTileCount = (m_MapX * m_MapY) * 0.42f;

	for (size_t i = 0; i < RandomTileCount; ++i)
	{
		auto iter = MapData.begin();
		auto iterEnd = MapData.end();

		ChangeTileImage(iter->second, LAND_STATE::SEA);

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
	int RandomTileCount = (m_MapX * m_MapY) * 0.51f;
	std::vector<Vector2> vecCreateIndex;
	Vector2 TileIndex = { 0.f,0.f };

	// ��� ���� 
	while (RandomTileCount)
	{
		// seed���� �̿��Ͽ� Ÿ���� ������ ��ġ�� ����
		TileIndex.x = rand() % m_MapX;
		TileIndex.y = rand() % m_MapY;

		if (vecCreateIndex.size() != 0)
		{
			// Vector�� ��ȸ�ϸ鼭 �̹� ����� �ε������ �ٽ� �������� ������
			for (int i = 0; i < vecCreateIndex.size(); ++i)
			{
				if ((vecCreateIndex[i].x == TileIndex.x) && (vecCreateIndex[i].x == TileIndex.y))
				{
					// �ٽ� �̰� �ٽ� ����
					TileIndex.x = (float)(rand() % m_MapX);
					TileIndex.y = (float)(rand() % m_MapY);

					i = 0;
				}
			}
		}

		// �̹� ������ �ε����� ����
		vecCreateIndex.push_back(TileIndex);

		// �ٴٷ� ����
		ChangeTileImage(TileIndex, LAND_STATE::SEA);

		--RandomTileCount;
	}
}

void CRandomMap::SmoothMap(float DeltaTime)
{
	if (m_IsGenerateFinished)
		return;

	for (int x = 0; x < m_MapX; ++x)
	{
		for (int y = 0; y < m_MapY; ++y)
		{
			int NearSeaCount = CheckNearSeaTile8(x, y);

			if (NearSeaCount > 4)
			{
				// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (���� ����)
				ChangeTileImage(Vector2(x, y), LAND_STATE::SEA);
			}

			else if (NearSeaCount < 4)
			{
				// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (������ ����)
				ChangeTileImage(Vector2(x, y), LAND_STATE::LAND);
			}
		}
	}
}

void CRandomMap::Clear(float DeltaTime)
{
	m_TileData.clear();

	for (int x = 0; x < m_MapX; ++x)
	{
		for (int y = 0; y < m_MapY; ++y)
		{
			// �ش� �κ��� Ÿ�ϸ� UV��ǥ�� ���� (������ ����)
			CTile* pTile = m_RandomMap->GetTile(Vector2((float)x, (float)y) * TILE_SIZE_SMALL);
			pTile->SetFrameStart(0.f, 0.f);
			pTile->SetFrameEnd(TILE_SIZE_SMALL, TILE_SIZE_SMALL);
		}
	}

	GenerateMapBase();
}


void CRandomMap::PickRandom()
{

}

void CRandomMap::ChangeTileImage(Vector2 tileIndex, LAND_STATE tileState)
{
	m_TileData[(int)tileIndex.x][(int)tileIndex.y] = tileState;

	CTile* pTile = m_RandomMap->GetTile(tileIndex * TILE_SIZE_SMALL);
	pTile->SetFrameStart(tileState * TILE_SIZE_SMALL, 0.f);
	pTile->SetFrameEnd((tileState + 1) * TILE_SIZE_SMALL, TILE_SIZE_SMALL);
}


int CRandomMap::CheckNearSeaTile8(int indexX, int indexY)
{
	int wallCount = 0;

	for (int nearX = indexX - 1; nearX <= indexX + 1; nearX++)
	{
		for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
		{
			// �� ���� üũ
			if (nearX >= 0 && nearX < m_MapX && nearY >= 0 && nearY < m_MapY)
			{
				if (nearX != m_MapX || nearY != m_MapY)
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
		if ((nearX >= 0 && nearX < m_MapX)&&
			m_TileData[nearX][indexY] == LAND_STATE::SEA)
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
		if ((nearY >= 0 && nearY < m_MapY) &&
			m_TileData[indexX][nearY] == LAND_STATE::SEA)
		{
			++wallCount;
		}
	}

	return wallCount;
}

std::vector<Vector2> CRandomMap::GetNearTileIndex(int indexX, int indexY, LAND_STATE checkTileState)
{
	std::vector<Vector2> result;

	return result;
}

std::vector<Vector2> CRandomMap::GetNearTileIndex(Vector2 index, LAND_STATE checkTileState)
{
	std::vector<Vector2> result;

	return result;
}

bool CRandomMap::CheckNearTileState4(Vector2 index, LAND_STATE checkTileState)
{
	bool result = false;

	if (m_TileData[index.x][index.y] == checkTileState)
	{
		result = true;
	}

	return result;
}

int CRandomMap::CheckNearTileState4(int indexX, int indexY, LAND_STATE checkTileState)
{
	int tileCount = 0;

	for (int nearX = indexX - 1; nearX <= indexX + 1; ++nearX)
	{
		if (nearX == indexX)
		{
			continue;
		}

		// �� ���� �̳����
		if ((nearX >= 0 && nearX < m_MapX) &&
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
		if ((nearY >= 0 && nearY < m_MapY) &&
			m_TileData[indexX][nearY] == checkTileState)
		{
			++tileCount;
		}
	}

	return tileCount;
}

int CRandomMap::CheckNearTileState(int indexX, int indexY, LAND_STATE checkTileState)
{
	int tileCount = 0;

	for (int nearX = indexX - 1; nearX <= indexX + 1; ++nearX)
	{
		if (nearX == indexX)
		{
			continue;
		}

		// �� ���� �̳����
		if ((nearX >= 0 && nearX < m_MapX) &&
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
		if ((nearY >= 0 && nearY < m_MapY) &&
			m_TileData[indexX][nearY] == checkTileState)
		{
			++tileCount;
		}
	}

	return tileCount;
}

