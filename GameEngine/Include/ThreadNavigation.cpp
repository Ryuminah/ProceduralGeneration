#include "ThreadNavigation.h"

CThreadNavigation::CThreadNavigation()
{
	m_hExitEvent = CreateEvent(0, FALSE, FALSE, 0);
}

CThreadNavigation::~CThreadNavigation()
{
	/*while (!m_WorkQueue.Empty())
	{
		NavWorkInfo* Info = nullptr;

		m_WorkQueue.Pop(Info);

		SAFE_DELETE(Info);
	}*/
}
