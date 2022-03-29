#pragma once

#include "GameEngine.h"
#include "Queue.h"

class CNavigationManager
{
private:
	Render_Space		m_RenderSpace;
	class CNavigation* m_Navigation;
	std::vector<class CThreadNavigation*>	m_vecThread;
	CQueue<NavMessage>	m_NavMsgQueue;

public:
	class CNavigation* GetNavigation()	const
	{
		return m_Navigation;
	}

	void AddMessage(class CSceneComponent* Component,
		const std::vector<Vector3>& vecPath)
	{
		NavMessage	Msg;
		Msg.Component = Component;
		Msg.vecPath = vecPath;

		m_NavMsgQueue.Push(Msg);
	}

public:
	bool Init();
	void CreateNavigation();
	void SetTileInfo();
	bool FindPath(const Vector3& Start, const Vector3& Goal,
		class CSceneComponent* Component);
	void Update(float DeltaTime);

	DECLARE_SINGLE(CNavigationManager)
};

