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

	// �ɼǵ��� ���� �����س��� (imgui���� �� �޾ƿ���)
	//�ش� �ɼ� ������ �ʿ��� Ÿ�ֿ̹� PerformOptionLevel �����σm
	std::unordered_map<std::string , class AMapGeneratorOption*> m_mapOptions;

private:
	int m_MapSizeX;
	int m_MapSizeY;
	bool m_IsGenerateWorldEnd;

	int m_SeaEndLineX;
	int m_SeaEndLineY;


	// �� Ÿ�ϵ��� LAND_STATE���� �з��س���
	// ���� ������ �Ϸ�� �� ���� Ÿ�� ������ �����ϴ� �ڷᱸ����
	std::map<TILE_STATE, std::vector<Vector2>> m_FinalTileStateData;

	// 2�����迭�� �����ϴ°� �³� ��
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
	// Algorithm
	void CellularAutomata();

	// �ɼǵ鵵 ������ ����� ���� Ŭ������ ����������
	void PerformSmootingLevel(OPTION_LEVEL smoothingLevel);
	void SmoothMap();
	void CreateRandom();

	// TileData
	void ChangeTileState(Vector2 tileIndex, TILE_STATE tileState);
	void ChangeTileStateData(Vector2 tileIndex, TILE_STATE tileState);

	// ���� �˰��� ¯���� ������ �Լ�ȭ ��Ű��...

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

