#pragma once
#include "GameObject.h"
#include "Component/TileMapComponent.h"
#include "../MapGenerator/IMapGenerator.h"
#include <map>
#define TILE_SIZE 64.f
#define TILE_SIZE_SMALL 16.f



class CRandomMap :
    public CGameObject
{
	friend class CScene;
	friend class CMapGenerator;

protected:
	CRandomMap();
	CRandomMap(const CRandomMap& obj);
	virtual ~CRandomMap();

protected:
	CSharedPtr<CTileMapComponent> m_RandomMap;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CRandomMap* Clone();


	// �⺻ ������ ������ �ְ� Ÿ�� �����ʹ� MapGenerator���� �����Ѵ�.
private:
	int m_MapX;
	int m_MapY;

	std::vector<std::vector<int>> m_TileData;
	std::map<LAND_STATE, std::vector<Vector2>> m_AllTileStateData;

	// �ٴ��� Ÿ�ϰ� ���� Ÿ�� ���� ���� �����س��´�.
	// �ش� �����ʹ� ,,, ���� �������� �����ϰ� ���� 
	std::vector<std::vector<int>> m_SeaTile;
	std::vector<std::vector<int>> m_LandTile;
	std::vector<std::vector<int>> m_LakeTile;
	std::vector<std::vector<int>> m_SandTile;

private:
	// �� ���¸� Ȯ������������ (�����͸� �����ϰ� ���� ���¸� Ȯ�� ���� ������ �����Ѵ�)
	bool m_IsGenerateFinished;

private:
	void GenerateMapBase();
	void GenerateEnvironment(float DeltaTime);
	
	//Random Algorithm
	void IndexRandomLogic();
	void RandomSaveLogic(); 
	void TestRandomLogic(); // �׽�Ʈ�� �ڵ� ���ɷ� ������

	void SmoothMap(float DeltaTime);
	void Clear(float DeltaTime);
	void SetOnlyLand(float DeltaTime);
	void SetGenerateFinished(float DeltaTime);

public:
	void CreateCoast(std::vector<Vector2> vecCoastTile);

private:
	// 
	void PickRandom();
	
private:
	void ChangeTileImage(Vector2 tileIndex, LAND_STATE tileState);
	int CheckNearSeaTile8(int indexX, int indexY);
	int CheckNearSeaTile4(int indexX, int indexY);

	// ���ڷ� �� TileState�� 4������ ��� �����ϴ���
	int CheckNearTileState4(int indexX, int indexY, LAND_STATE checkTileState);
	int CheckNearTileState(int indexX, int indexY, LAND_STATE checkTileState);

	std::vector<Vector2> GetNearTileIndex(int indexX, int indexY, LAND_STATE checkTileState);
	std::vector<Vector2>  GetNearTileIndex(Vector2 index, LAND_STATE checkTileState);

	// �ش� Ÿ���� �־����� �ش� Ÿ���� TILE_STATE�� �ε����� ��ȯ���ش�.
	bool CheckNearTileState4(Vector2 index, LAND_STATE checkTileState);



};

