#pragma once
#include <GameEngine.h>
#include "IMapGenerator.h"

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
	// 인자로 들어간 TileState가 4방향중 몇개나 존재하는지
	int Check_NearTileState8(int indexX, int indexY, TILE_STATE checkTileState);
	int Check_NearTileState8(Vector2 TileIndex, TILE_STATE checkTileState);

	int Check_NearTileState4(int indexX, int indexY, TILE_STATE checkTileState);
	int Check_NearTileState4(Vector2 Index, TILE_STATE checkTileState);

	bool Check_TileState(int indexX, int indexY, TILE_STATE checkTileState);

	// 주변 타일의 TILE_STATE를 조사해 인덱스를 가져옴
	std::vector<Vector2> GetNearTileState8(int indexX, int indexY, TILE_STATE checkTileState);
	std::vector<Vector2> GetNearTileState4(int indexX, int indexY, TILE_STATE checkTileState);
	std::vector<Vector2> GetNearTileState4(Vector2 Index, TILE_STATE checkTileState);


	// 싱글톤 굳이 ,, ?
};

