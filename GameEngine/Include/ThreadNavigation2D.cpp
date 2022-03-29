#include "ThreadNavigation2D.h"
#include "Component/TileMapComponent.h"
#include "Component/Tile.h"
#include "Navigation2D.h"
#include "NavigationManager.h"

CThreadNavigation2D::CThreadNavigation2D()
{
}

CThreadNavigation2D::~CThreadNavigation2D()
{
}

void CThreadNavigation2D::CreateNavigationInfo(CTileMapComponent* TileMap)
{
	int	TileCount = TileMap->GetTileCount();

	m_NavInfo.vecNavInfo.resize((size_t)TileCount);
	m_NavInfo.vecOpen.resize((size_t)TileCount);
	m_NavInfo.vecUse.resize((size_t)TileCount);

	m_NavInfo.CountX = TileMap->GetTileCountX();
	m_NavInfo.CountY = TileMap->GetTileCountY();

	for (int i = 0; i < TileCount; ++i)
	{
		CTile* Tile = TileMap->GetTile(i);

		m_NavInfo.vecNavInfo[i].Tile = Tile;
		m_NavInfo.vecNavInfo[i].IndexX = Tile->GetIndexX();
		m_NavInfo.vecNavInfo[i].IndexY = Tile->GetIndexY();
		m_NavInfo.vecNavInfo[i].Index = Tile->GetIndex();
	}
}

void CThreadNavigation2D::Run()
{
	NavWorkInfo Info;

	if (!m_WorkQueue.Pop(Info))
		return;

	((CNavigation2D*)m_Navigation)->SetNavInfoManager(&m_NavInfo);

	m_Navigation->FindPath(Info.Start, Info.Goal, m_vecResult);

	CNavigationManager::GetInst()->AddMessage(Info.CallbackComponent,
		m_vecResult);
}
