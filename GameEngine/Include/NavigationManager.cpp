#include "NavigationManager.h"
#include "Navigation2D.h"
#include "Navigation3D.h"
#include "Render/RenderManager.h"
#include "Component/TileMapComponent.h"
#include "ThreadManager.h"
#include "ThreadNavigation2D.h"

DEFINITION_SINGLE(CNavigationManager)

CNavigationManager::CNavigationManager()	:
	m_Navigation(nullptr)
{
}

CNavigationManager::~CNavigationManager()
{
	SAFE_DELETE(m_Navigation);
}

bool CNavigationManager::Init()
{
	return true;
}

void CNavigationManager::CreateNavigation()
{
	SAFE_DELETE(m_Navigation);

	m_RenderSpace = CRenderManager::GetInst()->GetRenderSpace();

	switch (m_RenderSpace)
	{
	case Render_Space::Render2D:
		m_Navigation = new CNavigation2D;
		break;
	case Render_Space::Render3D:
		m_Navigation = new CNavigation3D;
		break;
	}
}

void CNavigationManager::SetTileInfo()
{
	CTileMapComponent* TileMap = ((CNavigation2D*)m_Navigation)->GetTileMap();

	if (!TileMap)
		return;

	// 스레드를 생성한다.
	for (int i = 0; i < 4; ++i)
	{
		std::string	Name = "NavThread";
		Name += (i + 1);

		CThreadNavigation2D* Thread = CThreadManager::GetInst()->CreateThread<CThreadNavigation2D>(Name);

		Thread->SetNavigation(m_Navigation);
		Thread->CreateNavigationInfo(TileMap);

		m_vecThread.push_back(Thread);

		Thread->Start();
	}
}

bool CNavigationManager::FindPath(const Vector3& Start, const Vector3& Goal,
	CSceneComponent* Component)
{
	if (m_vecThread.empty())
		return false;

	// 스레드에서 현재 작업양이 가장 작은 스레드를 찾아서 해당 스레드에게 작업을
	// 할당해준다.
	CThreadNavigation* Thread = m_vecThread[0];

	size_t	Size = m_vecThread.size();

	for (size_t i = 1; i < Size; ++i)
	{
		if (Thread->GetWorkCount() > m_vecThread[i]->GetWorkCount())
			Thread = m_vecThread[i];
	}

	Thread->AddWorkInfo(Start, Goal, Component);

	return true;
}

void CNavigationManager::Update(float DeltaTime)
{
	while (!m_NavMsgQueue.Empty())
	{
		NavMessage	Msg;

		m_NavMsgQueue.Pop(Msg);

		Msg.Component->NavigationCallback(Msg.vecPath);
	}
}
