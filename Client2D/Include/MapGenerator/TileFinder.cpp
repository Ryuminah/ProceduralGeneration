#include "TileFinder.h"
#include "Scene/SceneResource.h"
#include "MapGenerator.h"
#include "RandomMap.h"

//DEFINITION_SINGLE(CTileFinder)

CTileFinder::CTileFinder()
{
}

CTileFinder::~CTileFinder()
{
}

int CTileFinder::Check_NearTileState8(int indexX, int indexY, TILE_STATE checkTileState, CMapGenerator* pCurrentMapGenerator)
{
	int tileCount = 0;

	int MapSizeX = pCurrentMapGenerator->m_pRandomMap->m_MapSizeX;
	int MapSizeY = pCurrentMapGenerator->m_pRandomMap->m_MapSizeY;

	for (int nearX = indexX - 1; nearX <= indexX + 1; nearX++)
	{
		for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
		{
			if (nearX != indexX && nearY != indexY)
			{
				if (Check_TileState(nearX, nearY, checkTileState, pCurrentMapGenerator))
					++tileCount;
			}
		}
	}

	return tileCount;
}

int CTileFinder::Check_NearTileState4(int indexX, int indexY, TILE_STATE checkTileState, CMapGenerator* pCurrentMapGenerator)
{
	int tileCount = 0;
	
	for (int nearX = indexX - 1; nearX <= indexX + 1; ++nearX)
	{
		if (nearX == indexX)
		{
			continue;
		}

		if (Check_TileState(nearX, indexY , checkTileState , pCurrentMapGenerator))
			++tileCount;
	}

	for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
	{
		if (nearY == indexY)
		{
			continue;
		}

		// 맵 범위 체크
		if (Check_TileState(indexX, nearY, checkTileState, pCurrentMapGenerator))
			++tileCount;
	}

	return tileCount;
}

bool CTileFinder::Check_TileState(int indexX, int indexY, TILE_STATE checkTileState, CMapGenerator* pCurrentMapGenerator)
{
	bool result = false;

	int MapSizeX = pCurrentMapGenerator->m_pRandomMap->m_MapSizeX;
	int MapSizeY = pCurrentMapGenerator->m_pRandomMap->m_MapSizeY;

	// 맵 범위 이내에서
	if (indexX >= 0 && indexX < MapSizeX &&
		indexY >= 0 && indexY < MapSizeY &&
		pCurrentMapGenerator->m_TileData[indexX][indexY] == checkTileState)
	{
		result = true;
	}

	return result;
}

std::vector<Vector2> CTileFinder::GetNearTileState8(int indexX, int indexY, TILE_STATE checkTileState, CMapGenerator* pCurrentMapGenerator)
{
	std::vector<Vector2> vecTileIndex;

	int MapSizeX = pCurrentMapGenerator->m_pRandomMap->m_MapSizeX;
	int MapSizeY = pCurrentMapGenerator->m_pRandomMap->m_MapSizeY;

	for (int nearX = indexX - 1; nearX <= indexX + 1; nearX++)
	{
		for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
		{
			if (nearX != indexX && nearY != indexY)
			{
				if (Check_TileState(nearX, nearY, checkTileState, pCurrentMapGenerator))
					vecTileIndex.push_back(Vector2(nearX, nearY));
			}
		}
	}

	return vecTileIndex;
}

std::vector<Vector2> CTileFinder::GetNearTileState4(int indexX, int indexY, TILE_STATE checkTileState, CMapGenerator* pCurrentMapGenerator)
{
	std::vector<Vector2> vecTileIndex;

	for (int nearX = indexX - 1; nearX <= indexX + 1; ++nearX)
	{
		if (nearX == indexX)
		{
			continue;
		}

		if (Check_TileState(nearX, indexY, checkTileState, pCurrentMapGenerator))
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
		if (Check_TileState(indexX, nearY, checkTileState, pCurrentMapGenerator))
		{
			vecTileIndex.push_back(Vector2(indexX, nearY));
		}
	}

	return vecTileIndex;
}

