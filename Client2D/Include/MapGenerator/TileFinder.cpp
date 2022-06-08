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

int CTileFinder::Check_NearTileState8(int indexX, int indexY, TILE_STATE checkTileState)
{
	int tileCount = 0;

	int MapSizeX = m_pOwner->GetMapSizeX();
	int MapSizeY = m_pOwner->GetMapSizeY();

	for (int nearX = indexX - 1; nearX <= indexX + 1; nearX++)
	{
		for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
		{
			if (nearX != indexX && nearY != indexY)
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
	int result = Check_NearTileState4((int)Index.x, (int)Index.x, checkTileState);

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
	bool result = Check_TileState((int)Index.x, (int)Index.x, checkTileState);

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
			if (nearX != indexX && nearY != indexY)
			{
				if (!CompareWith_TileState(indexX, indexY, nearX, nearY))
					++tileCount;
			}
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


std::vector<Vector2> CTileFinder::GetNearTileState8(int indexX, int indexY, TILE_STATE checkTileState)
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

std::vector<Vector2> CTileFinder::GetNearTileState4(int indexX, int indexY, TILE_STATE checkTileState)
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

std::vector<Vector2> CTileFinder::GetNearTileState4(Vector2 Index, TILE_STATE checkTileState)
{
	std::vector<Vector2> result = GetNearTileState4((int)Index.x, (int)Index.x, checkTileState);

	return result;
}

std::vector<Vector2> CTileFinder::GetAreaBorder(TILE_STATE checkTileState)
{
	std::vector<Vector2> vecResult;

	std::unordered_map<TILE_STATE, std::vector<Vector2>> tileStateData = m_pOwner->GetTileStateData();
	std::vector<std::vector<TILE_STATE>> tileData = m_pOwner->GetTileData();

	// 해당 TILE_STATE가 존재할 경우 영역의 가장자리를 체크하여 인덱스를 반환한다.
	if (tileStateData.end() != tileStateData.find(checkTileState))
	{
		for (size_t i = 0; i < tileStateData[checkTileState].size(); ++i)
		{
			// 하나라도 주변 타일의 TILE_STATE가 다르다면 가장자리로 간주한다.
			if (CompareWith_NearTileState8(tileStateData[checkTileState][i]) > 1)
			{
				vecResult.push_back(tileStateData[checkTileState][i]);
			}
		}
	}

	return vecResult;
}


bool CTileFinder::IsExistTile(int indexX, int indexY)
{
	Vector2 index = Vector2(indexX, indexY);
	bool result = IsExistTile(index);
	
	return result;
}

bool CTileFinder::IsExistTile(Vector2 tileIndex)
{
	bool result = true;
	Vector2 MapSize = m_pOwner->GetMapSize();

	if (tileIndex.x >= 0 && tileIndex.x < MapSize.x &&
		tileIndex.y >= 0 && tileIndex.y < MapSize.y)
	{
		result = false;
	}

	return result;
}

