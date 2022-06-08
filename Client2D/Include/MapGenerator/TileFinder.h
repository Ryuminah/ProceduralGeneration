#pragma once
#include <GameEngine.h>
#include "MapEnum.h"


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
	// �ֺ� Ÿ�� �� checkTileState Ÿ���� ������ ��ȯ
	int Check_NearTileState8(int indexX, int indexY, TILE_STATE checkTileState);
	int Check_NearTileState8(Vector2 TileIndex, TILE_STATE checkTileState);

	int Check_NearTileState4(int indexX, int indexY, TILE_STATE checkTileState);
	int Check_NearTileState4(Vector2 Index, TILE_STATE checkTileState);

	bool Check_TileState(int indexX, int indexY, TILE_STATE checkTileState);
	bool Check_TileState(Vector2 Index, TILE_STATE checkTileState);


	// ������ Ÿ�ϰ� TileState�� ������ �ٸ�, �ֺ� Ÿ���� ������ ��ȯ
	int CompareWith_NearTileState8(int indexX, int indexY);
	int CompareWith_NearTileState8(Vector2 index);

	int CompareWith_NearTileState4(int indexX, int indexY);
	int CompareWith_NearTileState4(Vector2 index);

	bool CompareWith_TileState(int srcIndexX, int srcIndexY , int destIndexX, int destIndexY);
	bool CompareWith_TileState(Vector2 srcIndex , Vector2 destIndex);

private:
	// �ֺ� Ÿ���� TILE_State�� ������ �ε����� ������
	std::vector<Vector2> GetNearTileState8(int indexX, int indexY, TILE_STATE checkTileState);
	std::vector<Vector2> GetNearTileState4(int indexX, int indexY, TILE_STATE checkTileState);
	std::vector<Vector2> GetNearTileState4(Vector2 Index, TILE_STATE checkTileState);

	// Area�� �����ڸ��� return ����
	std::vector<Vector2> GetAreaBorder(TILE_STATE checkTileState);
	std::vector<Vector2> Select_Border(TILE_STATE checkTileState);


	// �� �ȿ� �����ϴ� Ÿ������
	bool IsExistTile(int indexX, int indexY);
	bool IsExistTile(Vector2 tileIndex);
};

