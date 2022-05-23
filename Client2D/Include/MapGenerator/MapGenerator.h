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
	// 어떤 맵의 Generator인지 저장
	class CRandomMap* m_pRandomMap;

private:
	// 맵 생성기에 이게 있어야하나 ,, , ,?
	bool m_IsGenerateWorldEnd;

	// 각 타일들을 LAND_STATE별로 분류해놓음
	std::map<TILE_STATE, std::vector<Vector2>> m_AllTileStateData;
	std::vector<std::vector<TILE_STATE>> m_TileData;

public:
	// 곧 없어질 녀석 ,,,
	bool Init();
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

private:
	void CellularAutomata();
	void ChangeTileState(Vector2 tileIndex, TILE_STATE tileState);

};

