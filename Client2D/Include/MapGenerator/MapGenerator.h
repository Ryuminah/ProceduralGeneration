#pragma once
#include <GameEngine.h>
#include "../MapGenerator/IMapGenerator.h"

class CMapGenerator :
    public IMapGenerator
{
public:
	CMapGenerator();
	virtual ~CMapGenerator();

private:
	// 어떤 맵의 Generator인지 저장
	class CRandomMap* m_pRandomMap;

private:
	// 맵 생성기에 이게 있어야하나 ,, , ,?
	bool m_IsGenerateWorldEnd;
	int m_MapX;
	int m_MapY;

	std::vector<std::vector<int>> m_TileData;
	std::map<LAND_STATE, std::vector<Vector2>> m_AllTileStateData;

public:
    virtual void GenerateWorld(LAND_STATE _landState);
	virtual void GenerateVegetation(LAND_STATE _landState);

public:
	void GenerateBase();
	void GenerateLand();
	void GenerateSea();
	void GenerateCoast();
	void GenerateLake();
	void GenerateForest();

};

