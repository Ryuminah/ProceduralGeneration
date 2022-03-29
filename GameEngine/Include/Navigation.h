#pragma once

#include "GameEngine.h"
#include "Component/TileMapComponent.h"

class CNavigation
{
	friend class CNavigationManager;

protected:
	CNavigation();
	virtual ~CNavigation() = 0;

public:
	virtual float GetHeight(const Vector3& Pos);
	virtual bool FindPath(const Vector3& Start, const Vector3& Goal,
		std::vector<Vector3>& vecPath) = 0;
};

