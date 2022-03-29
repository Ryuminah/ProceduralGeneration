#pragma once
#include "Navigation.h"
class CNavigation3D :
    public CNavigation
{
	friend class CNavigationManager;

protected:
	CNavigation3D();
	virtual ~CNavigation3D();

protected:
	std::list<class CLandScapeComponent*>	m_LandScapeList;

public:
	void SetLandScape(class CLandScapeComponent* LandScape);
	void DeleteLandScape(class CLandScapeComponent* LandScape);

public:
	virtual float GetHeight(const Vector3& Pos);
	virtual bool FindPath(const Vector3& Start, const Vector3& Goal,
		std::vector<Vector3>& vecPath);
};

