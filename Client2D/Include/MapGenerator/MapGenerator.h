#pragma once
#include "IMapGenerator.h"

class CMapGenerator :
    public IMapGenerator
{
	friend class CTileFinder;

public:
	CMapGenerator();
	virtual ~CMapGenerator();

private:
	class CRandomMap*	m_pRandomMap;
	class CTileFinder*	m_pTileFinder;
	
private:
	int m_MapSizeX;
	int m_MapSizeY;
	bool m_IsGenerateWorldEnd;

	int m_SeaEndLineX;
	int m_SeaEndLineY;


	// 각 타일들을 LAND_STATE별로 분류해놓음
	// 월드 생성이 완료된 후 최종 타일 정보를 저장하는 자료구조임
	std::map<TILE_STATE, std::vector<Vector2>> m_FinalTileStateData;

	// 2차원배열로 저장하는거 맞냐 ㅠ
	std::vector<std::vector<TILE_STATE>> m_TileData;

public:
	bool Init(class CRandomMap* pRandomMap);


public:
    virtual void GenerateWorld(TILE_STATE _landState);
	virtual void GenerateVegetation(TILE_STATE _landState);

public:
	void GenerateBase();
	void GenerateLand();
	void GenerateSea();
	void GenerateCoast();
	void GenerateLake();
	void GenerateForest();

///////////////////////////////////// Private /////////////////////////////////////
private:
	void CellularAutomata();
	void ChangeTileState(Vector2 tileIndex, TILE_STATE tileState);
	void ChangeTileStateData(Vector2 tileIndex, TILE_STATE tileState);


///////////////////////////////////// Get, Set /////////////////////////////////////

public:
	int GetMapSizeX()
	{
		return m_MapSizeX;
	}

	int GetMapSizeY()
	{
		return m_MapSizeY;
	}

	Vector2 GetMapSize()
	{
		return Vector2(m_MapSizeX, m_MapSizeY);
	}

	std::vector<std::vector<TILE_STATE>> GetTileData()
	{
		return m_TileData;
	}
		

};

