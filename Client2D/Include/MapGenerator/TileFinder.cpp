#include <set>
#include "Scene/SceneResource.h"
#include "TileFinder.h"
#include "MapGenerator.h"
#include "RandomMap.h"

CTileFinder::CTileFinder() :m_pOwner(nullptr)
{
}

CTileFinder::CTileFinder(CMapGenerator* pMapGenerator) : m_pOwner(pMapGenerator)
{
	
}

CTileFinder::~CTileFinder()
{
}

int CTileFinder::Check_NearSeaTile8(int indexX, int indexY)
{
	int SeaCount = 0;
	int MapSizeX = m_pOwner->GetMapSizeX();
	int MapSizeY = m_pOwner->GetMapSizeY();


	for (int nearX = indexX - 1; nearX <= indexX + 1; nearX++)
	{
		for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
		{
			// 맵 범위 체크
			if (IsExistTile(nearX,nearY))
			{
				if (nearX != indexX || nearY != indexY)
				{
					SeaCount += m_pOwner->GetTileData()[nearX][nearY];
				}
			}

			// 인덱스를 초과한 타일은 전부 바다로 간주한다
			else
			{
				SeaCount++;
			}
		}
	}

	return SeaCount;
}

int CTileFinder::Check_NearTileState8(int indexX, int indexY, TILE_STATE checkTileState)
{
	int tileCount = 0;

	int MapSizeX = m_pOwner->GetMapSizeX();
	int MapSizeY = m_pOwner->GetMapSizeY();

	for (int nearX = indexX - 1; nearX <= indexX + 1; nearX++)
	{
		for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
		{
			if (nearX == indexX && nearY == indexY)
			{
				continue;
			}

			else
			{
				if (Check_TileState(nearX, nearY, checkTileState))
					++tileCount;
			}
		}
	}

	return tileCount;
}

int CTileFinder::Check_NearTileState8(Vector2 TileIndex, TILE_STATE checkTileState)
{
	int result = Check_NearTileState8(TileIndex.x, TileIndex.y, checkTileState);
	return result;
}

int CTileFinder::Check_NearTileState4(int indexX, int indexY, TILE_STATE checkTileState)
{
	int tileCount = 0;
	
	for (int nearX = indexX - 1; nearX <= indexX + 1; ++nearX)
	{
		if (nearX == indexX)
		{
			continue;
		}

		if (Check_TileState(nearX, indexY , checkTileState))
			++tileCount;
	}

	for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
	{
		if (nearY == indexY)
		{
			continue;
		}

		// 맵 범위 체크
		if (Check_TileState(indexX, nearY, checkTileState))
			++tileCount;
	}

	return tileCount;
}

int CTileFinder::Check_NearTileState4(Vector2 Index, TILE_STATE checkTileState)
{
	int result = Check_NearTileState4((int)Index.x, (int)Index.y, checkTileState);

	return result;
}

bool CTileFinder::Check_TileState(int indexX, int indexY, TILE_STATE checkTileState)
{
	bool result = false;

	// 맵 범위 이내에서
	if (IsExistTile(indexX,indexY)&&
		m_pOwner->GetTileData()[indexX][indexY] == checkTileState)
	{
		result = true;
	}

	return result;
}

bool CTileFinder::Check_TileState(Vector2 Index, TILE_STATE checkTileState)
{
	bool result = Check_TileState((int)Index.x, (int)Index.y, checkTileState);

	return result;
}

int CTileFinder::CompareWith_NearTileState8(int indexX, int indexY)
{
	int tileCount = 0;

	int MapSizeX = m_pOwner->GetMapSizeX();
	int MapSizeY = m_pOwner->GetMapSizeY();

	for (int nearX = indexX - 1; nearX <= indexX + 1; nearX++)
	{
		for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
		{
			if (nearX == indexX && nearY == indexY)
			{
				continue;
			}

			if (!CompareWith_TileState(indexX, indexY, nearX, nearY))
				++tileCount;
		}
	}

	return tileCount;
}

int CTileFinder::CompareWith_NearTileState8(Vector2 index)
{
	int result = CompareWith_NearTileState8((int)index.x, (int)index.y);
	return result;
}

int CTileFinder::CompareWith_NearTileState4(int indexX, int indexY)
{
	int tileCount = 0;

	for (int nearX = indexX - 1; nearX <= indexX + 1; ++nearX)
	{
		if (nearX == indexX)
		{
			continue;
		}

		if (!CompareWith_TileState(indexX, indexY, nearX, indexY))
			++tileCount;
	}

	for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
	{
		if (nearY == indexY)
		{
			continue;
		}

		// 맵 범위 체크
		if (!CompareWith_TileState(indexX, indexY, indexX, nearY))
			++tileCount;
	}

	return tileCount;
}

int CTileFinder::CompareWith_NearTileState4(Vector2 index)
{
	int result = CompareWith_NearTileState4((int)index.x , (int)index.y);
	return result;
}

bool CTileFinder::CompareWith_TileState(int srcIndexX, int srcIndexY, int destIndexX, int destIndexY)
{
	Vector2 srcIndex = Vector2(srcIndexX, srcIndexY);
	Vector2 destIndex = Vector2(destIndexX, destIndexY);

	return CompareWith_TileState(srcIndex, destIndex);
}

bool CTileFinder::CompareWith_TileState(Vector2 srcIndex, Vector2 destIndex)
{
	if (!(IsExistTile(srcIndex) || IsExistTile(destIndex))) return false;

	bool result = false;
	TILE_STATE srcTileState = m_pOwner->GetTileData()[srcIndex.x][srcIndex.y];
	TILE_STATE destTileState = m_pOwner->GetTileData()[destIndex.x][destIndex.y];

	result = srcTileState == destTileState ? true : false;

	return result;
}


std::vector<Vector2> CTileFinder::Get_NearTileState8(int indexX, int indexY, TILE_STATE checkTileState)
{
	std::vector<Vector2> vecTileIndex;

	int MapSizeX = m_pOwner->GetMapSizeX();
	int MapSizeY = m_pOwner->GetMapSizeY();

	for (int nearX = indexX - 1; nearX <= indexX + 1; nearX++)
	{
		for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
		{
			if (nearX != indexX && nearY != indexY)
			{
				if (Check_TileState(nearX, nearY, checkTileState))
					vecTileIndex.push_back(Vector2(nearX, nearY));
			}
		}
	}

	return vecTileIndex;
}

std::vector<Vector2> CTileFinder::Get_NearTileState4(int indexX, int indexY, TILE_STATE checkTileState)
{
	std::vector<Vector2> vecTileIndex;

	for (int nearX = indexX - 1; nearX <= indexX + 1; ++nearX)
	{
		if (nearX == indexX)
		{
			continue;
		}

		if (Check_TileState(nearX, indexY, checkTileState))
		{
			vecTileIndex.push_back(Vector2(nearX, indexY));
		}
	}

	for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
	{
		if (nearY == indexY)
		{
			continue;
		}

		// 맵 범위 체크
		if (Check_TileState(indexX, nearY, checkTileState))
		{
			vecTileIndex.push_back(Vector2(indexX, nearY));
		}
	}

	return vecTileIndex;
}

std::vector<Vector2> CTileFinder::Get_NearTileState4(Vector2 Index, TILE_STATE checkTileState)
{
	std::vector<Vector2> result = Get_NearTileState4((int)Index.x, (int)Index.y, checkTileState);
	return result;
}

std::vector<Vector2> CTileFinder::Get_NearTileIndex8(int indexX, int indexY)
{
	std::vector<Vector2> vecTileIndex;

	int MapSizeX = m_pOwner->GetMapSizeX();
	int MapSizeY = m_pOwner->GetMapSizeY();

	for (int nearX = indexX - 1; nearX <= indexX + 1; nearX++)
	{
		for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
		{
			if (nearX != indexX && nearY != indexY)
			{
					vecTileIndex.push_back(Vector2(nearX, nearY));
			}
		}
	}

	return vecTileIndex;
}

std::vector<Vector2> CTileFinder::Get_NearTileIndex8(Vector2 Index)
{
	std::vector<Vector2> result = Get_NearTileIndex8((int)Index.x, (int)Index.y);

	return result;
}

std::vector<Vector2> CTileFinder::Get_NearTileIndex4(int indexX, int indexY)
{
	std::vector<Vector2> vecTileIndex;

	for (int nearX = indexX - 1; nearX <= indexX + 1; ++nearX)
	{
		if (nearX == indexX || !IsExistTile(nearX, indexY))
		{
			continue;
		}

		vecTileIndex.push_back(Vector2(nearX, indexY));
	}

	for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
	{
		if (nearY == indexY || !IsExistTile(indexX, nearY))
		{
			continue;
		}

		vecTileIndex.push_back(Vector2(indexX, nearY));
	}

	return vecTileIndex;
}

std::vector<Vector2> CTileFinder::Get_NearTileIndex4(Vector2 Index)
{
	std::vector<Vector2> result = Get_NearTileIndex4((int)Index.x, (int)Index.y);
	return result;
}

std::vector<Vector2> CTileFinder::Get_AreaBorder(TILE_STATE checkTileState)
{
	std::vector<Vector2> vecResult;

	// 항상 갱신되는 TileData를 기준으로 Area를 탐색해야함
	std::vector<std::vector<TILE_STATE>> vecTileData = m_pOwner->GetTileData();
	std::vector<Vector2> tileStateData;

	for (int x = 0; x < m_pOwner->GetMapSizeX(); ++x)
	{
		for (int  y = 0; y < m_pOwner->GetMapSizeY(); ++y)
		{
			if (vecTileData[x][y] == checkTileState)
			{
				tileStateData.push_back(Vector2(float(x), float(y)));
			}
		}
	}

	// 해당 Area가 존재할 경우
	if (tileStateData.size() > 0)
	{
		for (size_t i = 0; i < tileStateData.size(); ++i)
		{
			if (CompareWith_NearTileState4(tileStateData[i]) > 0)
			{
				vecResult.push_back(tileStateData[i]);
			}
		}
	}

	return vecResult;
}

std::vector<Vector2> CTileFinder::Get_AreaBorder(std::vector<Vector2> areaIndex)
{
	std::vector<Vector2> vecResult;
	std::set<std::pair<float, float>> setAreaIndex;		// 탐색을 위하여 ...

	// x,y의 최대 최솟값을 구한다
	Vector2 minIndex = {m_pOwner->GetMapSize()};
	Vector2 maxIndex = {0.f,0.f};

	for (size_t i = 0; i < areaIndex.size(); ++i)
	{
		Vector2 currentIndex = areaIndex[i];
		setAreaIndex.insert(std::make_pair(areaIndex[i].x, areaIndex[i].y));

		// 최솟값
		if (currentIndex.x < minIndex.x)
		{
			minIndex.x = currentIndex.x;
		}

		else if (currentIndex.x > maxIndex.x)
		{
			maxIndex.x = currentIndex.x;
		}

		// 최솟값
		if (currentIndex.y < minIndex.y)
		{
			minIndex.y = currentIndex.y;
		}

		else if (currentIndex.y > maxIndex.y)
		{
			maxIndex.y = currentIndex.y;
		}
	}

	// 세로 가장자리
	// 플롯으로 갈겨버린 이유는 어차피 정수라서 귀찮아서 , ,, ,, ,ㅠ
	for (float x = minIndex.x; x <= maxIndex.x; ++x)
	{
		float minY = maxIndex.y;
		float maxY = minIndex.y;

		for (float y = minIndex.y; y <= maxIndex.y; ++y)
		{
			auto findIter = setAreaIndex.find(std::make_pair(x, y));

			// 존재하는 타일이라면
			if (findIter != setAreaIndex.end())
			{
				Vector2 compareIndex = Vector2(findIter->first, findIter->second);

				if (compareIndex.y < minY)
				{
					minY = compareIndex.y;
				}

				else if (compareIndex.y > maxY)
				{
					maxY = compareIndex.y;
				}
			}

			else
			{
				int a = 0;
			}
		}

		Vector2 heightMinIndex = { x, minY };
		Vector2 heightMaxIndex = { x, maxY };
		Vector2 heightIndex[2] = { heightMinIndex ,heightMaxIndex };

		for (int i = 0; i < 2; ++i)
		{
			if (!Find_TileIndex(heightIndex[i], vecResult))
			{
				vecResult.push_back(heightIndex[i]);
			}
		}
	}


	// 가로 가장자리
	for (float y = minIndex.y; y <= maxIndex.y; ++y)
	{
		float minX = maxIndex.x;
		float maxX = minIndex.x;

		for (float x = minIndex.x; x <= maxIndex.x; ++x)
		{
			auto findIter = setAreaIndex.find(std::make_pair(x, y));

			// 존재하는 타일이라면
			if (findIter != setAreaIndex.end())
			{
				Vector2 compareIndex = Vector2(findIter->first, findIter->second);

				if (compareIndex.x < minX)
				{
					minX = compareIndex.x;
				}

				else if (compareIndex.x > maxX)
				{
					maxX = compareIndex.x;
				}
			}
		}

		Vector2 widthMinIndex = { minX, y};
		Vector2 widthMaxIndex = { maxX, y};
		Vector2 widthIndex[2] = { widthMinIndex,widthMaxIndex };


		for (int i = 0; i < 2; ++i)
		{
			if (!Find_TileIndex(widthIndex[i], vecResult))
			{
				vecResult.push_back(widthIndex[i]);
			}
		}
	}

	return vecResult;
}

std::vector<Vector2> CTileFinder::Get_AreaBorder(std::vector<Vector2> areaIndex, TILE_STATE checkTileState)
{
	std::vector<Vector2> vecResult;

	// 해당 Area가 존재할 경우
	for (size_t i = 0; i < areaIndex.size(); ++i)
	{
		if (CompareWith_NearTileState4(areaIndex[i]) > 0)
		{
			vecResult.push_back(areaIndex[i]);
		}
	}

	return vecResult;
}

std::vector<Vector2> CTileFinder::Get_OutlineTiles(std::vector<Vector2> tiles)
{
	std::vector<Vector2> vecOutLine;

	// 해당 타일들을 감싸는 테두리 타일을 원하는 두께 만큼 리턴 
	// 8방향을 조사해서 
	// 1. 인자로 들어온 타일이 아니어야하고,
	// 2. 중복이 되면 안됨.
	
	// 이미 저장된 타일일 경우 지나감.
	std::set<std::pair<float, float>> setNearTileIndex;
	std::set<std::pair<float, float>> setTiles;

	std::vector<Vector2> vecBorderTiles = Get_AreaBorder(tiles);

	for (size_t i = 0; i < vecBorderTiles.size(); ++i)
	{
		std::vector<Vector2> nearTileIndex = Get_NearTileIndex4(vecBorderTiles[i]);

		for (size_t j = 0; j < nearTileIndex.size(); j++)
		{
			if (!Find_TileIndex(nearTileIndex[j], tiles)&&
				!Find_TileIndex(nearTileIndex[j], vecOutLine))
			{
				vecOutLine.push_back(nearTileIndex[j]);
			}
		}
		
	}

	return vecOutLine;
}

bool CTileFinder::Find_TileIndex(Vector2 srcIndex, std::vector<Vector2> destTiles)
{
	// 탐색 속도 비교
	bool result = false;

	// 같은 영역으로 묶여있다면 TILE_STATE가 달라도 경계로 판단하지 않는다.
	for (size_t i = 0; i < destTiles.size(); ++i)
	{
		if (destTiles[i] == srcIndex)
		{
			result = true;
		}
	}
	
	return result;
}

bool CTileFinder::IsExistTile(int indexX, int indexY)
{
	Vector2 index = Vector2(indexX, indexY);
	bool result = IsExistTile(index);
	
	return result;
}

bool CTileFinder::IsExistTile(Vector2 tileIndex)
{
	bool result = false;
	Vector2 MapSize = m_pOwner->GetMapSize();

	if (tileIndex.x >= 0 && (tileIndex.x < MapSize.x) &&
		tileIndex.y >= 0 && (tileIndex.y < MapSize.y))
	{
		result = true;
	}

	return result;
}

