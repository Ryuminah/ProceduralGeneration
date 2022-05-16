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


	// 기본 정보만 가지고 있고 타일 데이터는 MapGenerator에서 관리한다.
private:
	int m_MapX;
	int m_MapY;

	std::vector<std::vector<int>> m_TileData;
	std::map<LAND_STATE, std::vector<Vector2>> m_AllTileStateData;

	// 바다인 타일과 땅인 타일 등을 따로 저장해놓는다.
	// 해당 데이터는 ,,, 맵을 최종으로 생성하고 나서 
	std::vector<std::vector<int>> m_SeaTile;
	std::vector<std::vector<int>> m_LandTile;
	std::vector<std::vector<int>> m_LakeTile;
	std::vector<std::vector<int>> m_SandTile;

private:
	// 땅 형태를 확정지을것인지 (데이터를 저장하고 나면 형태를 확정 지은 것으로 간주한다)
	bool m_IsGenerateFinished;

private:
	void GenerateMapBase();
	void GenerateEnvironment(float DeltaTime);
	
	//Random Algorithm
	void IndexRandomLogic();
	void RandomSaveLogic(); 
	void TestRandomLogic(); // 테스트용 코드 렉걸려 쓰지마

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

	// 인자로 들어간 TileState가 4방향중 몇개나 존재하는지
	int CheckNearTileState4(int indexX, int indexY, LAND_STATE checkTileState);
	int CheckNearTileState(int indexX, int indexY, LAND_STATE checkTileState);

	std::vector<Vector2> GetNearTileIndex(int indexX, int indexY, LAND_STATE checkTileState);
	std::vector<Vector2>  GetNearTileIndex(Vector2 index, LAND_STATE checkTileState);

	// 해당 타일을 넣었을때 해당 타일이 TILE_STATE면 인덱스를 반환해준다.
	bool CheckNearTileState4(Vector2 index, LAND_STATE checkTileState);



};

