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
	int Check_NearSeaTile8(int indexX, int indexY);

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
	// 주변 타일의 TILE_State를 조사해 해당 TILE_STATE의 인덱스를 가져옴
	// 모든 Get 함수는 데이터가 항상 갱신되고 있는 있는 m_TileState를 기반으로 제작할 것
	std::vector<Vector2> Get_NearTileState8(int indexX, int indexY, TILE_STATE checkTileState);
	std::vector<Vector2> Get_NearTileState4(int indexX, int indexY, TILE_STATE checkTileState);
	std::vector<Vector2> Get_NearTileState4(Vector2 Index, TILE_STATE checkTileState);

	std::vector<Vector2> Get_NearTileIndex8(int indexX, int indexY);
	std::vector<Vector2> Get_NearTileIndex8(Vector2 Index);
	std::vector<Vector2> Get_NearTileIndex4(int indexX, int indexY);
	std::vector<Vector2> Get_NearTileIndex4(Vector2 Index);


	// Area의 가장자리를 return 해줌
	std::vector<Vector2> Get_AreaBorder(TILE_STATE checkTileState);

	// 해당 타일들을 감싸는 테두리 타일을 리턴
	std::vector<Vector2> Get_OutlineTiles(std::vector<Vector2> tiles);


	//std::vector<Vector2> Select_Border(TILE_STATE checkTileState);		// 이거 뭐만들려고 했던거더라 ,,,?


	// 맵 안에 존재하는 타일인지
	bool IsExistTile(int indexX, int indexY);
	bool IsExistTile(Vector2 tileIndex);

public:
	void SetOwner(class CMapGenerator* pMapGenerator)
	{
		m_pOwner = pMapGenerator;
	}
};



