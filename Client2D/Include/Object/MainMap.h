#pragma once

#include "GameObject.h"
#include "Component/TileMapComponent.h"

class CMainMap :
    public CGameObject
{
	friend class CScene;

protected:
	CMainMap();
	CMainMap(const CMainMap& obj);
	virtual ~CMainMap();

protected:
	CSharedPtr<CTileMapComponent> m_TileMap;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CMainMap* Clone();
};

