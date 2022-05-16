#pragma once
#include "Navigation.h"
class CNavigation3D :
    public CNavigation
{
	friend class CNavigationManager;

protected:
	CNavigation3D();
	virtual ~CNavigation3D();

public:
	virtual bool FindPath(const Vector3& Start, const Vector3& Goal,
		std::vector<Vector3>& vecPath);
};

