#pragma once

#include "Navigation.h"

enum Neighbor_Dir
{
	ND_Top,
	ND_RightTop,
	ND_Right,
	ND_RightBottom,
	ND_Bottom,
	ND_LeftBottom,
	ND_Left,
	ND_LeftTop,
	ND_End
};

class CNavigation2D :
    public CNavigation
{
	friend class CNavigationManager;

protected:
	CNavigation2D();
	virtual ~CNavigation2D();

private:
	CSharedPtr<CTileMapComponent>	m_NavData;
	NavInfoManager* m_InfoManager;

public:
	CTileMapComponent* GetTileMap()	const
	{
		return m_NavData;
	}

	void SetNavData(CTileMapComponent* Data)
	{
		m_NavData = Data;
	}

	void SetNavInfoManager(NavInfoManager* Info)
	{
		m_InfoManager = Info;
	}

public:
	virtual bool FindPath(const Vector3& Start, const Vector3& Goal,
		std::vector<Vector3>& vecPath);

private:
	bool FindNode(NavInfo* Node, NavInfo* GoalNode, const Vector3& Goal,
		std::vector<Vector3>& vecPath);

private:
	static bool SortOpenList(NavInfo* Src, NavInfo* Dest);
};

