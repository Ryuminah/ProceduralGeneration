#pragma once
#include <map>
#include "Component/TileMapComponent.h"
#include "GameObject.h"
#include "MapEnum.h"

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

private:
	int m_MapSizeX;
	int m_MapSizeY;
	
private:
	// Generate World
	void GenerateBase(float DeltaTime);
	void GenerateLand(float DeltaTime);
	void GenerateCoast(float DeltaTime);
	void GenerateForest(float DeltaTime);
	void GenerateLake(float DeltaTime);
	void Clear(float DeltaTime);

private:
	void GenerateEnvironment(float DeltaTime);
	
public:
	Vector2 SetMapSize(Vector2 mapSize)
	{
		m_MapSizeX = mapSize.x;
		m_MapSizeY = mapSize.y;
	}
};

