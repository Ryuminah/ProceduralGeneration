#pragma once
#include <GameEngine.h>
#include "IMapGenerator.h"

// MapGenerator�� ���ؼ��� ������ �� �ְ���
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
	// ���ڷ� �� TileState�� 4������ ��� �����ϴ���
	int Check_NearTileState8(int indexX, int indexY, TILE_STATE checkTileState);
	int Check_NearTileState8(Vector2 TileIndex, TILE_STATE checkTileState);

	int Check_NearTileState4(int indexX, int indexY, TILE_STATE checkTileState);
	int Check_NearTileState4(Vector2 Index, TILE_STATE checkTileState);

	bool Check_TileState(int indexX, int indexY, TILE_STATE checkTileState);

	// �ֺ� Ÿ���� TILE_STATE�� ������ �ε����� ������
	std::vector<Vector2> GetNearTileState8(int indexX, int indexY, TILE_STATE checkTileState);
	std::vector<Vector2> GetNearTileState4(int indexX, int indexY, TILE_STATE checkTileState);
	std::vector<Vector2> GetNearTileState4(Vector2 Index, TILE_STATE checkTileState);


	// �̱��� ���� ,, ?
};

