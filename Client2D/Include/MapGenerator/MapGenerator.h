#pragma once
#include <GameEngine.h>
#include "../MapGenerator/IMapGenerator.h"

class CMapGenerator :
    public IMapGenerator
{
	friend class CTileFinder;

public:
	CMapGenerator();
	virtual ~CMapGenerator();

private:
	// � ���� Generator���� ����
	class CRandomMap* m_pRandomMap;


private:
	// �� �����⿡ �̰� �־���ϳ� ,, , ,?
	bool m_IsGenerateWorldEnd;

	std::map<LAND_STATE, std::vector<Vector2>> m_AllTileStateData;
	std::vector<std::vector<LAND_STATE>> m_TileData;

public:
	// �� ������ �༮ ,,,
	bool Init();
	bool Init(class CRandomMap* pRandomMap);


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

private:
	void CellularAutomata();
};

