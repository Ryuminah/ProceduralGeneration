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
	class CRandomMap* m_pRandomMap;
	class CTileFinder* m_pTileFinder;
	class CGenerateWindow* m_GenerateWindow;


	// 옵션들을 전부 저장해놓고 (imgui에서 값 받아오기)
	//해당 옵션 설정이 필요한 타이밍에 PerformOptionLevel 수행하m
	std::unordered_map<std::string, class AMapGeneratorOption*> m_mapOptions;

private:
	bool m_IsGenerateWorldEnd;

private:
	int m_MapSizeX;
	int m_MapSizeY;
	int m_OutLineX;
	int m_OutLineY;

	int m_ForestMinX;
	int m_ForestMinY;
	int m_ForestMaxX;
	int m_ForestMaxY;

	int m_LakeMinX;
	int m_LakeMinY;
	int m_LakeMaxX;
	int m_LakeMaxY;

	// 각 타일들을 LAND_State별로 분류해놓음
	// 매 생성단계가 끝난 후 재 분류
	std::unordered_map<TILE_STATE, std::vector<Vector2>> m_TileStateData;
	std::vector<std::vector<TILE_STATE>> m_TileData;

private:
	TILE_STATE currentTileState;

public:
	bool Init(class CRandomMap* pRandomMap);

public:
	virtual void GenerateWorld(TILE_STATE _landState);
	virtual void GenerateVegetation(TILE_STATE _landState);
	void GenerateWorld();

private:
	void GenerateBase();
	void GenerateLand();
	void GenerateSea();
	void GenerateCoast();
	void GenerateLake();
	void GenerateForest();
	
	void ClearAll();

	///////////////////////////////////// Private /////////////////////////////////////
private:
	// Algorithm
	void CellularAutomata();
	void UpdgradeCellularAutomata();


	// 옵션들도 각자의 기능을 가진 클래스로 빼야할지도
	void PerformSmootingLevel(OPTION_LEVEL smoothingLevel);
	void SmoothMap();
	void CreateRandom();
	void ResetWorld();

	// TileData
	void ChangeTileStateImage(Vector2 tileIndex, TILE_STATE tileState);		// 타일 정보를 갱신하며 이미지를 교체함
	void ChangeTileData(Vector2 tileIndex, TILE_STATE tileState);		

	// 타일 데이터를 전부 갱신한다.
	void UpdateTileStateData();

	// 랜덤 알고리즘 짱구좀 굴려서 함수화 시키기...

private:
	void DebugFunctionTime(void(CMapGenerator::*pFunc)());

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

	std::unordered_map<TILE_STATE, std::vector<Vector2>> GetTileStateData()
	{
		return m_TileStateData;
	}

	void SetCurrentTileState(TILE_STATE tileState)
	{
		currentTileState = tileState;
	}
};