#pragma once
#include <map>
#include "Component/TileMapComponent.h"
#include "GameObject.h"
#include "MapEnum.h"

// ��� ����
//#include "IMapGenerator.h"

#define TILE_SIZE 64.f
#define TILE_SIZE_SMALL 16.f

class CRandomMap :
    public CGameObject
{
	friend class CScene;
	friend class CMapGenerator;
	friend class CTileFinder;


protected:
	CRandomMap();
	CRandomMap(const CRandomMap& obj);
	virtual ~CRandomMap();

private:
	class CMapGenerator* m_MapGenerator;
	CSharedPtr<CTileMapComponent> m_MapComponent;

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
	int m_MapSizeX;
	int m_MapSizeY;

	std::vector<std::vector<int>> m_TileData;
	std::map<TILE_STATE, std::vector<Vector2>> m_AllTileStateData;

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
	
public:
	void ChangeTileImage(Vector2 tileIndex, TILE_STATE tileState);
	int CheckNearSeaTile8(int indexX, int indexY);
	int CheckNearSeaTile4(int indexX, int indexY);

	// ���ڷ� �� TileState�� 4������ ��� �����ϴ���
	int CheckNearTileState4(int indexX, int indexY, TILE_STATE checkTileState);
	bool CheckNearTileState4(Vector2 index, TILE_STATE checkTileState);
	int CheckNearTileState(int indexX, int indexY, TILE_STATE checkTileState);

};

