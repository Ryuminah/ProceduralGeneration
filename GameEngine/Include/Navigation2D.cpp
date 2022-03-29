#include "Navigation2D.h"

CNavigation2D::CNavigation2D()	:
	m_InfoManager(nullptr)
{
}

CNavigation2D::~CNavigation2D()
{
}

bool CNavigation2D::FindPath(const Vector3& Start, const Vector3& Goal,
	std::vector<Vector3>& vecPath)
{
	if (!m_NavData)
		return false;

	// 시작 타일과 도착타일의 인덱스를 얻어온다.
	int	GoalIndex = m_NavData->GetTileIndex(Goal);

	if (GoalIndex == -1)
		return false;

	int	StartIndex = m_NavData->GetTileIndex(Start);

	if (StartIndex == -1)
		return false;

	vecPath.clear();

	NavInfo* StartInfo = &m_InfoManager->vecNavInfo[StartIndex];
	NavInfo* GoalInfo = &m_InfoManager->vecNavInfo[GoalIndex];

	if (StartInfo == GoalInfo)
	{
		vecPath.push_back(Goal);
		return true;
	}

	m_InfoManager->UseCount = 0;
	m_InfoManager->OpenCount = 0;

	m_InfoManager->vecOpen[m_InfoManager->OpenCount] = StartInfo;
	++m_InfoManager->OpenCount;

	m_InfoManager->vecUse[m_InfoManager->UseCount] = StartInfo;
	++m_InfoManager->UseCount;
	m_InfoManager->vecUse[m_InfoManager->UseCount] = GoalInfo;
	++m_InfoManager->UseCount;

	NavInfo* Node = nullptr;

	while (m_InfoManager->OpenCount > 0)
	{
		--m_InfoManager->OpenCount;
		Node = m_InfoManager->vecOpen[m_InfoManager->OpenCount];

		// 닫힌목록으로 만들어준다.
		Node->Type = Nav_Insert_Type::Close;

		// 주변 8방향 노드를 얻어온다.
		// 아래 함수는 길을 찾았을 경우 경로를 만들어주고 true를 반환한다.
		if (FindNode(Node, GoalInfo, Goal, vecPath))
			break;

		// 아직 길을 찾아야 한다면 열린목록에 들어가 있는 노드들을 정렬해주도록 한다.
		std::sort(m_InfoManager->vecOpen.begin(),
			m_InfoManager->vecOpen.begin() + m_InfoManager->OpenCount,
			CNavigation2D::SortOpenList);
	}

	// 길찾기가 끝났다면 사용했던 노드들을 초기화 시켜준다.
	for (int i = 0; i < m_InfoManager->UseCount; ++i)
	{
		m_InfoManager->vecUse[i]->Clear();
	}

	return !vecPath.empty();
}

bool CNavigation2D::FindNode(NavInfo* Node, NavInfo* GoalNode, 
	const Vector3& Goal, std::vector<Vector3>& vecPath)
{
	// 8방향의 이웃노드를 구하기 위해 x와 y의 인덱스를 구한다.
	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	// 타일 1개의 크기를 구한다.
	const Vector2& TileSize = Node->Tile->GetSize();

	NavInfo* Neighbor[ND_End] = {};

	// 현재 타일의 모양에 따라 8방향 노드를 구해주는 방법이 달라진다.
	// 그러므로 타일의 모양에 따라 8방향의 노드를 구해주는 switch문을
	// 작성한다.
	switch (m_NavData->GetTileShape())
	{
	case Tile_Shape::Rect:
		Neighbor[ND_Top] = m_InfoManager->GetNavInfo(IndexX, IndexY + 1);
		Neighbor[ND_RightTop] = m_InfoManager->GetNavInfo(IndexX + 1, IndexY + 1);
		Neighbor[ND_Right] = m_InfoManager->GetNavInfo(IndexX + 1, IndexY);
		Neighbor[ND_RightBottom] = m_InfoManager->GetNavInfo(IndexX + 1, IndexY - 1);
		Neighbor[ND_Bottom] = m_InfoManager->GetNavInfo(IndexX, IndexY - 1);
		Neighbor[ND_LeftBottom] = m_InfoManager->GetNavInfo(IndexX - 1, IndexY - 1);
		Neighbor[ND_Left] = m_InfoManager->GetNavInfo(IndexX - 1, IndexY);
		Neighbor[ND_LeftTop] = m_InfoManager->GetNavInfo(IndexX - 1, IndexY + 1);
		break;
	case Tile_Shape::Rhombus:
		if (IndexY % 2 == 0)
		{
			Neighbor[ND_RightTop] = m_InfoManager->GetNavInfo(IndexX, IndexY + 1);
			Neighbor[ND_RightBottom] = m_InfoManager->GetNavInfo(IndexX, IndexY - 1);
			Neighbor[ND_LeftBottom] = m_InfoManager->GetNavInfo(IndexX - 1, IndexY - 1);
			Neighbor[ND_LeftTop] = m_InfoManager->GetNavInfo(IndexX - 1, IndexY + 1);
		}

		else
		{
			Neighbor[ND_RightTop] = m_InfoManager->GetNavInfo(IndexX + 1, IndexY + 1);
			Neighbor[ND_RightBottom] = m_InfoManager->GetNavInfo(IndexX + 1, IndexY - 1);
			Neighbor[ND_LeftBottom] = m_InfoManager->GetNavInfo(IndexX, IndexY - 1);
			Neighbor[ND_LeftTop] = m_InfoManager->GetNavInfo(IndexX, IndexY + 1);
		}

		Neighbor[ND_Top] = m_InfoManager->GetNavInfo(IndexX, IndexY + 2);
		Neighbor[ND_Bottom] = m_InfoManager->GetNavInfo(IndexX, IndexY - 2);
		Neighbor[ND_Left] = m_InfoManager->GetNavInfo(IndexX - 1, IndexY);
		Neighbor[ND_Right] = m_InfoManager->GetNavInfo(IndexX + 1, IndexY);
		break;
	}

	// 이웃의 수만큼 반복한다.
	for (int i = 0; i < ND_End; ++i)
	{
		if (!Neighbor[i])
			continue;

		// 이웃노드가 갈 수 없는 벽이라면 체크를 제외한다.
		if (Neighbor[i]->Tile->GetTileType() == Tile_Type::Wall)
			continue;

		// 이웃노드가 닫힌목록에 들어간 노드라면 제외한다.
		if (Neighbor[i]->Type == Nav_Insert_Type::Close)
			continue;

		Neighbor_Dir	Dir1 = ND_End, Dir2 = ND_End;

		if (m_NavData->GetTileShape() == Tile_Shape::Rect)
		{
			switch (i)
			{
			case ND_RightTop:
				Dir1 = ND_Top;
				Dir2 = ND_Right;
				break;
			case ND_RightBottom:
				Dir1 = ND_Bottom;
				Dir2 = ND_Right;
				break;
			case ND_LeftTop:
				Dir1 = ND_Top;
				Dir2 = ND_Left;
				break;
			case ND_LeftBottom:
				Dir1 = ND_Bottom;
				Dir2 = ND_Left;
				break;
			}
		}

		else
		{
			switch (i)
			{
			case ND_Top:
				Dir1 = ND_RightTop;
				Dir2 = ND_LeftTop;
				break;
			case ND_Bottom:
				Dir1 = ND_RightBottom;
				Dir2 = ND_LeftBottom;
				break;
			case ND_Left:
				Dir1 = ND_LeftTop;
				Dir2 = ND_LeftBottom;
				break;
			case ND_Right:
				Dir1 = ND_RightBottom;
				Dir2 = ND_RightTop;
				break;
			}
		}

		if (Dir1 != ND_End || Dir2 != ND_End)
		{
			if (Neighbor[Dir1] && Neighbor[Dir2])
			{
				if (Neighbor[Dir1]->Tile->GetTileType() == Tile_Type::Wall &&
					Neighbor[Dir2]->Tile->GetTileType() == Tile_Type::Wall)
					continue;
			}

			else if (Neighbor[Dir1])
			{
				if (Neighbor[Dir1]->Tile->GetTileType() == Tile_Type::Wall)
					continue;
			}

			else if (Neighbor[Dir2])
			{
				if (Neighbor[Dir2]->Tile->GetTileType() == Tile_Type::Wall)
					continue;
			}
		}

		// 도착했는지 판단하고 아니라면 열린목록을 지정해준다.
		if (Neighbor[i] == GoalNode)
		{
			// 기존 경로를 제거한다.
			vecPath.clear();

			// 도착 위치를 넣어준다.
			vecPath.push_back(Goal);

			NavInfo* Parent = Node;

			while (Parent)
			{
				Vector2	Center = Parent->Tile->GetCenter();

				vecPath.push_back(Vector3(Center.x, Center.y, 0.f));

				Parent = Parent->Parent;
			}

			vecPath.pop_back();

			return true;
		}

		// 현재의 이웃노드로부터 도착점 까지의 직선 거리를 구한다.
		Vector2	Center = Neighbor[i]->Tile->GetCenter();
		Vector2	CenterDist = Center - Vector2(Goal.x, Goal.y);

		float	Dist = CenterDist.x * CenterDist.x + CenterDist.y * CenterDist.y;

		float	Cost = 0.f;

		// 방향에 따라 간선 비용을 구해준다.
		// 간선 비용은 타일의 모양에 따라서 달라진다.
		if (m_NavData->GetTileShape() == Tile_Shape::Rect)
		{
			switch (i)
			{
			case ND_Top:
			case ND_Bottom:
				Cost = Node->Cost + Neighbor[i]->Tile->GetDistH();
				break;
			case ND_Right:
			case ND_Left:
				Cost = Node->Cost + Neighbor[i]->Tile->GetDistW();
				break;
			default:
				Cost = Node->Cost + Neighbor[i]->Tile->GetDistD();
				break;
			}
		}

		else
		{
			switch (i)
			{
			case ND_Top:
			case ND_Bottom:
				Cost = Node->Cost + Neighbor[i]->Tile->GetDistH();
				break;
			case ND_Right:
			case ND_Left:
				Cost = Node->Cost + Neighbor[i]->Tile->GetDistW();
				break;
			default:
				Cost = Node->Cost + Neighbor[i]->Tile->GetDistDHalf();
				break;
			}
		}

		// 만약 현재의 이웃노드가 열린목록에 이미 들어가 있는 노드일 경우에는
		// 기존의 비용과 현재 구해준 비용을 비교하여 더 작은 비용으로
		// 교체를 해주도록 한다.
		if (Neighbor[i]->Type == Nav_Insert_Type::Open)
		{
			if (Neighbor[i]->Cost > Cost)
			{
				Neighbor[i]->Parent = Node;
				Neighbor[i]->Cost = Cost;
				Neighbor[i]->Dist = Dist;
				Neighbor[i]->Total = Cost + Dist;
			}
		}

		// 열린 목록에 들어간 노드가 아니라면 비용을 대입하여 열린목록에 넣어주도록 한다.
		else
		{
			m_InfoManager->vecUse[m_InfoManager->UseCount] = Neighbor[i];
			++m_InfoManager->UseCount;

			Neighbor[i]->Type = Nav_Insert_Type::Open;
			Neighbor[i]->Parent = Node;
			Neighbor[i]->Cost = Cost;
			Neighbor[i]->Dist = Dist;
			Neighbor[i]->Total = Cost + Dist;

			m_InfoManager->vecOpen[m_InfoManager->OpenCount] = Neighbor[i];
			++m_InfoManager->OpenCount;
		}
	}

	return false;
}

bool CNavigation2D::SortOpenList(NavInfo* Src, NavInfo* Dest)
{
	return Src->Total > Dest->Total;
}
