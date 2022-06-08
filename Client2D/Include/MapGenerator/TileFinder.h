#pragma once
#include <GameEngine.h>
#include "MapEnum.h"


// MapGenerator를 통해서만 접근할 수 있게함
class CTileFinder
{
	friend class CMapGenerator;

private:
	class CMapGenerator* m_pOwner;

private:
	CTileFinder();
	CTileFinder(class CMapGenerator* pMapGenerator);
	~CTileFinder();


private:
	// 주변 타일 중 checkTileState 타일의 갯수를 반환
	int Check_NearTileState8(int indexX, int indexY, TILE_STATE checkTileState);
	int Check_NearTileState8(Vector2 TileIndex, TILE_STATE checkTileState);

	int Check_NearTileState4(int indexX, int indexY, TILE_STATE checkTileState);
	int Check_NearTileState4(Vector2 Index, TILE_STATE checkTileState);

	bool Check_TileState(int indexX, int indexY, TILE_STATE checkTileState);
	bool Check_TileState(Vector2 Index, TILE_STATE checkTileState);


	// 조사할 타일과 TileState의 종류가 다른, 주변 타일의 갯수를 반환
	int CompareWith_NearTileState8(int indexX, int indexY);
	int CompareWith_NearTileState8(Vector2 index);

	int CompareWith_NearTileState4(int indexX, int indexY);
	int CompareWith_NearTileState4(Vector2 index);

	bool CompareWith_TileState(int srcIndexX, int srcIndexY , int destIndexX, int destIndexY);
	bool CompareWith_TileState(Vector2 srcIndex , Vector2 destIndex);

private:
	// 주변 타일의 TILE_State를 조사해 인덱스를 가져옴
	std::vector<Vector2> GetNearTileState8(int indexX, int indexY, TILE_STATE checkTileState);
	std::vector<Vector2> GetNearTileState4(int indexX, int indexY, TILE_STATE checkTileState);
	std::vector<Vector2> GetNearTileState4(Vector2 Index, TILE_STATE checkTileState);

	// Area의 가장자리를 return 해줌
	std::vector<Vector2> GetAreaBorder(TILE_STATE checkTileState);
	std::vector<Vector2> Select_Border(TILE_STATE checkTileState);


	// 맵 안에 존재하는 타일인지
	bool IsExistTile(int indexX, int indexY);
	bool IsExistTile(Vector2 tileIndex);
};

