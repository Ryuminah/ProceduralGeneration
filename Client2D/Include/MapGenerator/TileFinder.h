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

	// ���ڷ� �� TileState�� 4������ ��� �����ϴ���
	int CheckNearTileState4(int indexX, int indexY, TILE_STATE checkTileState);
	int CheckNearTileState(int indexX, int indexY, TILE_STATE checkTileState);

	std::vector<Vector2> GetNearTileIndex(int indexX, int indexY, TILE_STATE checkTileState);
	std::vector<Vector2>  GetNearTileIndex(Vector2 index, TILE_STATE checkTileState);

	// �ش� Ÿ���� �־����� �ش� Ÿ���� TILE_STATE�� �ε����� ��ȯ���ش�.
	bool CheckNearTileState4(class CTileMapComponent* tileMapComponent Vector2 index, TILE_STATE checkTileState);
};

