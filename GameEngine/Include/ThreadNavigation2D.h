#pragma once

#include "ThreadNavigation.h"
#include "Sync.h"
#include "Component/Tile.h"

class CThreadNavigation2D :
    public CThreadNavigation
{
	friend class CThreadManager;

protected:
	CThreadNavigation2D();
	virtual ~CThreadNavigation2D();

private:
	NavInfoManager	m_NavInfo;

public:
	int GetWorkCount()	const
	{
		return m_WorkQueue.Size();
	}


public:
	void CreateNavigationInfo(class CTileMapComponent* TileMap);

public:
	virtual void Run();
};

