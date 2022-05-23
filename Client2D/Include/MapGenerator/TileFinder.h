#pragma once
#include <GameEngine.h>
#include "IMapGenerator.h"

class CTileFinder
{
public:
	CTileFinder();
	~CTileFinder();

public:
	// ����Ǹ� ��������� ���� ���� ������ �ִ°� �������� . . . .. ..^^:;
	// ���ڷ� �� TileState�� 4������ ��� �����ϴ���
	int Check_NearTileState8(int indexX, int indexY, TILE_STATE checkTileState, class CMapGenerator* pCurrentMapGenerator);
	int Check_NearTileState4(int indexX, int indexY, TILE_STATE checkTileState, class CMapGenerator* pCurrentMapGenerator);
	bool Check_TileState(int indexX, int indexY, TILE_STATE checkTileState, class CMapGenerator* pCurrentMapGenerator);

	// �ֺ� Ÿ���� TILE_STATE�� ������ �ε����� ������
	std::vector<Vector2> GetNearTileState8(int indexX, int indexY, TILE_STATE checkTileState, class CMapGenerator* pCurrentMapGenerator);
	std::vector<Vector2> GetNearTileState4(int indexX, int indexY, TILE_STATE checkTileState, class CMapGenerator* pCurrentMapGenerator);

	//DECLARE_SINGLE(CTileFinder)
};

