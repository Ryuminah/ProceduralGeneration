#pragma once
#include <GameEngine.h>
#include "IMapGenerator.h"

class CTileFinder
{
public:
	CTileFinder();
	~CTileFinder();

public:
	// 이쯤되면 멤버변수로 현재 맵을 가지고 있는게 나을지도 . . . .. ..^^:;
	// 인자로 들어간 TileState가 4방향중 몇개나 존재하는지
	int Check_NearTileState8(int indexX, int indexY, TILE_STATE checkTileState, class CMapGenerator* pCurrentMapGenerator);
	int Check_NearTileState4(int indexX, int indexY, TILE_STATE checkTileState, class CMapGenerator* pCurrentMapGenerator);
	bool Check_TileState(int indexX, int indexY, TILE_STATE checkTileState, class CMapGenerator* pCurrentMapGenerator);

	// 주변 타일의 TILE_STATE를 조사해 인덱스를 가져옴
	std::vector<Vector2> GetNearTileState8(int indexX, int indexY, TILE_STATE checkTileState, class CMapGenerator* pCurrentMapGenerator);
	std::vector<Vector2> GetNearTileState4(int indexX, int indexY, TILE_STATE checkTileState, class CMapGenerator* pCurrentMapGenerator);

	//DECLARE_SINGLE(CTileFinder)
};

