#include "TileFinder.h"
#include "Scene/SceneResource.h"
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
	Check_NearTileState8(TileIndex.x, TileIndex.y, checkTileState);
	return 0;
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

	int MapSizeX = m_pOwner->GetMapSizeX();
	int MapSizeY = m_pOwner->GetMapSizeY();


	// 맵 범위 이내에서
	if (indexX >= 0 && indexX < MapSizeX &&
		indexY >= 0 && indexY < MapSizeY &&
		m_pOwner->GetTileData()[indexX][indexY] == checkTileState)
	{
		result = true;
	}

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

