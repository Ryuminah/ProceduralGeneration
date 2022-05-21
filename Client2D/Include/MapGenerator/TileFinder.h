#pragma once
#include <GameEngine.h>
#include "IMapGenerator.h"

class CTileFinder
{
public:
	CTileFinder();
	~CTileFinder();

public:
	int CheckNearSeaTile8(int indexX, int indexY);
	int CheckNearSeaTile4(int indexX, int indexY);

	// 인자로 들어간 TileState가 4방향중 몇개나 존재하는지
	int CheckNearTileState4(int indexX, int indexY, TILE_STATE checkTileState);
	int CheckNearTileState(int indexX, int indexY, TILE_STATE checkTileState);

	std::vector<Vector2> GetNearTileIndex(int indexX, int indexY, TILE_STATE checkTileState);
	std::vector<Vector2>  GetNearTileIndex(Vector2 index, TILE_STATE checkTileState);

	// 해당 타일을 넣었을때 해당 타일이 TILE_STATE면 인덱스를 반환해준다.
	bool CheckNearTileState4(class CTileMapComponent* tileMapComponent Vector2 index, TILE_STATE checkTileState);
};

