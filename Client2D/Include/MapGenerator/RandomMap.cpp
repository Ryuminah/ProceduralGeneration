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

	clock_t start = clock(); // 시작 시간 저장
	
	IndexRandomLogic();
	
	// 이건 진짜 생각이 안나는군 ㄱㅡ
	//RandomSaveLogic();		


	clock_t end = clock(); 
	double clockTime = (double)(end - start) / CLOCKS_PER_SEC;
	char buffer[BUFSIZ];
	sprintf_s(buffer, "%f", clockTime);


	// 후에 맵의 가장 가장자리를 물로 변경한다
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

	// 걸린 시간 출력
	// 단위: 초(second)
	// CLOCKS_PER_SEC로 나눠줘야 초단위로 나옵니다.

	CEngine::GetInst()->AddDebugLog(buffer);
}


void CRandomMap::GenerateEnvironment(float DeltaTime)
{
	// 땅, 바다 조성 이후 환경 조성

	// 땅 가생이를 판단한다.
}

void CRandomMap::SetOnlyLand(float DeltaTime)
{
	// 주변 타일 4개이상 땅이면 땅으로 바꿈 (최대한 땅으로 바꿈)
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
	// 현재의 맵 기반을 고정시킨 뒤 환경을 형성한다
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
	
	// Land 중 강가에 있는 타일 체크 후 모래로 한번 바꿈.
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

	// 샌드 만들고 랜덤으로 ...
	for (size_t i = 0; i < vecCoastTile.size(); ++i)
	{
		Vector2 currentSandTile = vecCoastTile[i];

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

		ChangeTileImage(TileIndex, TILE_STATE::COAST);
		//m_AllTileStateData.insert(std::pair<TILE_STATE, std::vector<Vector2>>(TILE_STATE::COAST, vecCoastTile));
		vecCoastTile.push_back(TileIndex);

		// 랜덤 인덱스의 value를 End로 교체, 가장 마지막 값 삭제
		auto iterEnd = mapRandomSandTile.end();
		mapRandomSandTile[RandomSeed] = (--iterEnd)->second;
		mapRandomSandTile.erase(iterEnd);

		--RandomTileCount;
	}

	m_AllTileStateData.insert(std::pair<TILE_STATE, std::vector<Vector2>>(TILE_STATE::COAST, vecCoastTile));
}


void CRandomMap::IndexRandomLogic()
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
		//RandomSeed = rand() % RandomIndex.size();
		std::uniform_int_distribution<int> dist(0, MapIndex.size());
		RandomSeed = dist(randomDevice);

	 	//Vector2 TileIndex = RandomIndex[RandomSeed];
		Vector2 TileIndex = MapIndex[RandomSeed];

		// 해당 부분의 타일만 UV좌표를 변경 (물로 변경)
		ChangeTileImage(TileIndex, TILE_STATE::SEA);

		// 랜덤 인덱스의 value를 End로 교체, 가장 마지막 값 삭제
		auto iterEnd = MapIndex.end();
		MapIndex[RandomSeed] = (--iterEnd)->second;
		MapIndex.erase(iterEnd);

		--RandomTileCount;
	}
}

void CRandomMap::RandomSaveLogic()
{
	// 맵 크기만큼 리스트나 맵에 저장해놓고
	// 당첨 번호의 노드를 삭제
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

			// 실제 저장될 키 값이 second
			RandomIndex[Index] = Index;
			++Index;
		}
	}
	                              
	while (RandomIndex.size() != 0)
	{
		std::uniform_int_distribution<int> dist(0, RandomIndex.size());
		RandomSeed = dist(randomDevice);

		//Vector2 TileIndex = RandomIndex[RandomSeed];
		// 실제 인덱스를 저장해놓은 second의 값으로 진짜 TileIndex를 탐색
		
		Vector2 TileIndex = MapIndex[RandomIndex[RandomSeed]];

		// 랜덤으로 집어넣음
		MapData[RandomIndex[RandomSeed]] = TileIndex;

		// 인덱스를 삭제
		RandomIndex.erase(RandomSeed);
	}

	// 50퍼의 비율을 물로 채움
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
	//	// seed값을 이용하여 타일을 랜덤한 위치에 생성
	//	// 남은 타일의 갯수 중 인덱스 선정
	//	//RandomSeed = rand() % RandomIndex.size();
	//	RandomSeed = dist(randomDevice);

	//	//Vector2 TileIndex = RandomIndex[RandomSeed];
	//	Vector2 TileIndex = RandomIndex[RandomSeed];

	//	// 해당 부분의 타일만 UV좌표를 변경 (물로 변경)
	//	ChangeTileImage(TileIndex, LAND_STATE::SEA);

	//	// 랜덤 인덱스의 value를 End로 교체, 가장 마지막 값 삭제
	//	auto iterEnd = RandomIndex.end();
	//	RandomIndex[RandomSeed] = (--iterEnd)->second;
	//	RandomIndex.erase(iterEnd);

	//	//RandomIndex.erase(RandomSeed);


	//	--RandomTileCount;
	//}
}


void CRandomMap::TestRandomLogic()
{
	// 랜덤한 위치에 물 타일을 생성한다.
	srand((unsigned int)time(NULL));

	// 50퍼의 비율을 물로 채움
	int RandomTileCount = (m_MapSizeX * m_MapSizeY) * 0.51f;
	std::vector<Vector2> vecCreateIndex;
	Vector2 TileIndex = { 0.f,0.f };

	// 방식 변경 
	while (RandomTileCount)
	{
		// seed값을 이용하여 타일을 랜덤한 위치에 생성
		TileIndex.x = rand() % m_MapSizeX;
		TileIndex.y = rand() % m_MapSizeY;

		if (vecCreateIndex.size() != 0)
		{
			// Vector를 순회하면서 이미 변경된 인덱스라면 다시 랜덤값을 선정함
			for (int i = 0; i < vecCreateIndex.size(); ++i)
			{
				if ((vecCreateIndex[i].x == TileIndex.x) && (vecCreateIndex[i].x == TileIndex.y))
				{
					// 다시 뽑고 다시 조사
					TileIndex.x = (float)(rand() % m_MapSizeX);
					TileIndex.y = (float)(rand() % m_MapSizeY);

					i = 0;
				}
			}
		}

		// 이미 생성한 인덱스는 저장
		vecCreateIndex.push_back(TileIndex);

		// 바다로 변경
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
				// 해당 부분의 타일만 UV좌표를 변경 (물로 변경)
				ChangeTileImage(Vector2(x, y), TILE_STATE::SEA);
			}

			else if (NearSeaCount < 4)
			{
				// 해당 부분의 타일만 UV좌표를 변경 (땅으로 변경)
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
			// 해당 부분의 타일만 UV좌표를 변경 (땅으로 변경)
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
			// 맵 범위 체크
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

		// 맵 범위 이내라면
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

		// 맵 범위 체크
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

		// 맵 범위 이내라면
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

		// 맵 범위 체크
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

		// 맵 범위 이내라면
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

		// 맵 범위 체크
		if ((nearY >= 0 && nearY < m_MapSizeY) &&
			m_TileData[indexX][nearY] == checkTileState)
		{
			++tileCount;
		}
	}

	return tileCount;
}

