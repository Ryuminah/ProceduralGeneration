#pragma once
#include "GameObject.h"
#include "Component/TileMapComponent.h"

class CRandomMap :
    public CGameObject
{
	friend class CScene;

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
};

