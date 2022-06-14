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
	int wallCount = 0;
	int MapSizeX = m_pOwner->GetMapSizeX();
	int MapSizeY = m_pOwner->GetMapSizeY();


	for (int nearX = indexX - 1; nearX <= indexX + 1; nearX++)
	{
		for (int nearY = indexY - 1; nearY <= indexY + 1; nearY++)
		{
			// �� ���� üũ
			if (nearX >= 0 && nearX < MapSizeX && nearY >= 0 && nearY < MapSizeY)
			{
				if (nearX != MapSizeX || nearY != MapSizeY)
				{
					wallCount += m_pOwner->GetTileData()[nearX][nearY];
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

		// �� ���� üũ
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

	// �� ���� �̳�����
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

		// �� ���� üũ
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

		// �� ���� üũ
		if (Check_TileState(indexX, nearY, checkTileState))
		{
			vecTileIndex.push_back(Vector2(indexX, nearY));
		}
	}

	return vecTileIndex;
}

std::vector<Vector2> CTileFinder::Get_NearTileState4(Vector2 Index, TILE_STATE checkTileState)
{
	std::vector<Vector2> result = Get_NearTileState4((int)Index.x, (int)Index.x, checkTileState);
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
	std::vector<Vector2> result = Get_NearTileIndex8((int)Index.x, (int)Index.x);

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
	std::vector<Vector2> result = Get_NearTileIndex4((int)Index.x, (int)Index.x);
	return result;
}

std::vector<Vector2> CTileFinder::Get_AreaBorder(TILE_STATE checkTileState)
{
	std::vector<Vector2> vecResult;

	std::unordered_map<TILE_STATE, std::vector<Vector2>> tileStateData = m_pOwner->GetTileStateData();
	std::vector<std::vector<TILE_STATE>> tileData = m_pOwner->GetTileData();

	// �ش� TILE_STATE�� ������ ��� ������ �����ڸ��� üũ�Ͽ� �ε����� ��ȯ�Ѵ�.
	if (tileStateData.end() != tileStateData.find(checkTileState))
	{
		for (size_t i = 0; i < tileStateData[checkTileState].size(); ++i)
		{
			// �ϳ��� �ֺ� Ÿ���� TILE_STATE�� �ٸ��ٸ� �����ڸ��� �����Ѵ�.
			if (CompareWith_NearTileState8(tileStateData[checkTileState][i]) > 0)
			{
				vecResult.push_back(tileStateData[checkTileState][i]);
			}
		}
	}

	return vecResult;
}

std::vector<Vector2> CTileFinder::Get_OutlineTiles(std::vector<Vector2> tiles)
{
	std::vector<Vector2> vecOutLine;

	// �ش� Ÿ�ϵ��� ���δ� �׵θ� Ÿ���� ���ϴ� �β� ��ŭ ���� 
	// 8������ �����ؼ� 
	// 1. ���ڷ� ���� Ÿ���� �ƴϾ���ϰ�,
	// 2. �ߺ��� �Ǹ� �ȵ�.
	
	// �̹� ����� Ÿ���� ��� ������.
	std::set<std::pair<float, float>> setNearTileIndex;
	std::set<std::pair<float, float>> setTiles;

	// ���ڷ� ���� Ÿ���� �ε����� ���� ������
	for (size_t i = 0; i < tiles.size(); ++i)
	{
		setTiles.insert(std::make_pair(tiles[i].x, tiles[i].y));
	}

	for (auto iter = setTiles.begin(); iter != setTiles.end(); ++iter)
	{
		// ���ڷ� ���� Ÿ���߿� �ϳ���� ����
		Vector2 currentIndex = { iter->first,iter->second };
		std::vector<Vector2> nearTileIndex8 = Get_NearTileIndex8(currentIndex);

		for (size_t k = 0; k < nearTileIndex8.size(); ++k)
		{
			Vector2 nearIndex = { nearTileIndex8[k].x , nearTileIndex8[k].y };
			auto TileIter = setTiles.find(std::pair<float, float>(nearIndex.x, nearIndex.y));
			
			// �̹� �ִ°� �ƴϸ� �ֱ�
			if (TileIter != setTiles.end())
			{
				Vector2 Index = { nearTileIndex8[k].x,nearTileIndex8[k].y };
				setNearTileIndex.insert(std::make_pair((int)Index.x, (int)Index.y));
			}
		}
	}
	
	for (auto iter = setTiles.begin(); iter != setTiles.end(); ++iter)
	{
		vecOutLine.push_back(Vector2(iter->first, iter->second));
	}

	return vecOutLine;
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

