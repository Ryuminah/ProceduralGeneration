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

	clock_t start = clock(); // 시작 시간 저장
	
	IndexRandomLogic();
	//TestRandomLogic();
	
	// 이건 진짜 생각이 안나는군 ㄱㅡ
	//RandomSaveLogic();		


	clock_t end = clock(); 
	double clockTime = (double)(end - start) / CLOCKS_PER_SEC;
	char buffer[BUFSIZ];
	sprintf_s(buffer, "%f", clockTime);


	// 후에 맵의 가장 가장자리를 물로 변경한다
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
	// 현재의 맵 기반을 고정시킨 뒤 환경을 형성한다
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
	
	// Land 중 강가에 있는 타일 체크 후 모래로 한번 바꿈.
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

	//// 한번 더 반복 (이거 야매코드 수정하기..)
	//for (size_t i = 0; i < vecCoastTile.size(); ++i)
	//{
	//	Vector2 currentSandTile = vecCoastTile[i];

	//	for (int nearX = currentSandTile.x - 1; nearX <= currentSandTile.x + 1; ++nearX)
	//	{
	//		if (nearX == currentSandTile.x)
	//		{
	//			continue;
	//		}

	//		// 맵 범위 이내의 타일이 모래가 아니라면 저장한다
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

	//		// 맵 범위 체크
	//		if ((nearY >= 0 && nearY < m_MapY) &&
	//			m_TileData[currentSandTile.x][nearY])
	//		{
	//			m_TileData[currentSandTile.x][nearY] != LAND_STATE::COAST;
	//			mapRandomSandTile.insert(std::pair<int, Vector2>(keyIndex, Vector2(currentSandTile.x, nearY)));
	//			++keyIndex;
	//		}
	//	}
	//}

	//// 랜덤으로 뽑아서 . .. . . 샌드 타일 만들기

	//RandomTileCount = mapRandomSandTile.size() * 0.5f;
	//RandomSeed = 0;

	//while (RandomTileCount)
	//{
	//	// seed값을 이용하여 타일을 랜덤한 위치에 생성
	//	// 남은 타일의 갯수 중 인덱스 선정
	//	std::uniform_int_distribution<int> dist(0, mapRandomSandTile.size());
	//	RandomSeed = dist(randomDevice);

	//	Vector2 TileIndex = mapRandomSandTile[RandomSeed];

	//	// 해당 부분의 타일만 UV좌표를 변경 (물로 변경)
	//	ChangeTileImage(TileIndex, LAND_STATE::COAST);

	//	// 랜덤 인덱스의 value를 End로 교체, 가장 마지막 값 삭제
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

			// 맵 범위 체크
			if ((nearY >= 0 && nearY < m_MapY) &&
				m_TileData[currentSandTile.x][nearY])
			{
				m_TileData[currentSandTile.x][nearY] != LAND_STATE::COAST;
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

		// 해당 부분의 타일만 UV좌표를 변경 (물로 변경)
		ChangeTileImage(TileIndex, LAND_STATE::COAST);
		m_AllTileStateData.insert(std::pair<LAND_STATE, std::vector<Vector2>>(LAND_STATE::COAST, vecCoastTile));
		vecCoastTile.push_back(TileIndex);

		// 랜덤 인덱스의 value를 End로 교체, 가장 마지막 값 삭제
		auto iterEnd = mapRandomSandTile.end();
		mapRandomSandTile[RandomSeed] = (--iterEnd)->second;
		mapRandomSandTile.erase(iterEnd);

		--RandomTileCount;
	}

}


void CRandomMap::IndexRandomLogic()
{
	// 맵 크기만큼 리스트나 맵에 저장해놓고
	// 당첨 번호의 노드를 삭제
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
		// seed값을 이용하여 타일을 랜덤한 위치에 생성
		// 남은 타일의 갯수 중 인덱스 선정
		//RandomSeed = rand() % RandomIndex.size();
		std::uniform_int_distribution<int> dist(0, MapIndex.size());
		RandomSeed = dist(randomDevice);

	 	//Vector2 TileIndex = RandomIndex[RandomSeed];
		Vector2 TileIndex = MapIndex[RandomSeed];

		// 해당 부분의 타일만 UV좌표를 변경 (물로 변경)
		ChangeTileImage(TileIndex, LAND_STATE::SEA);

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

	for (int x = 0; x < m_MapX; ++x)
	{
		for (int y = 0; y < m_MapY; ++y)
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
	int RandomTileCount = (m_MapX * m_MapY) * 0.51f;
	std::vector<Vector2> vecCreateIndex;
	Vector2 TileIndex = { 0.f,0.f };

	// 방식 변경 
	while (RandomTileCount)
	{
		// seed값을 이용하여 타일을 랜덤한 위치에 생성
		TileIndex.x = rand() % m_MapX;
		TileIndex.y = rand() % m_MapY;

		if (vecCreateIndex.size() != 0)
		{
			// Vector를 순회하면서 이미 변경된 인덱스라면 다시 랜덤값을 선정함
			for (int i = 0; i < vecCreateIndex.size(); ++i)
			{
				if ((vecCreateIndex[i].x == TileIndex.x) && (vecCreateIndex[i].x == TileIndex.y))
				{
					// 다시 뽑고 다시 조사
					TileIndex.x = (float)(rand() % m_MapX);
					TileIndex.y = (float)(rand() % m_MapY);

					i = 0;
				}
			}
		}

		// 이미 생성한 인덱스는 저장
		vecCreateIndex.push_back(TileIndex);

		// 바다로 변경
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
				// 해당 부분의 타일만 UV좌표를 변경 (물로 변경)
				ChangeTileImage(Vector2(x, y), LAND_STATE::SEA);
			}

			else if (NearSeaCount < 4)
			{
				// 해당 부분의 타일만 UV좌표를 변경 (땅으로 변경)
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
			// 해당 부분의 타일만 UV좌표를 변경 (땅으로 변경)
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
			// 맵 범위 체크
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

		// 맵 범위 이내라면
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

		// 맵 범위 체크
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

		// 맵 범위 이내라면
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

		// 맵 범위 체크
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

		// 맵 범위 이내라면
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

		// 맵 범위 체크
		if ((nearY >= 0 && nearY < m_MapY) &&
			m_TileData[indexX][nearY] == checkTileState)
		{
			++tileCount;
		}
	}

	return tileCount;
}

