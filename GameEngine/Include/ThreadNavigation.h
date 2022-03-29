#pragma once

#include "Thread.h"
#include "Queue.h"

struct NavWorkInfo
{
	Vector3	Start;
	Vector3	Goal;
	class CSceneComponent* CallbackComponent;
};

class CThreadNavigation :
    public CThread
{
	friend class CThreadManager;

protected:
	CThreadNavigation();
	virtual ~CThreadNavigation() = 0;

protected:
	class CNavigation* m_Navigation;
	CQueue<NavWorkInfo, 500>	m_WorkQueue;
	std::vector<Vector3>		m_vecResult;
	HANDLE			m_hExitEvent;

public:
	int GetWorkCount()	const
	{
		return m_WorkQueue.Size();
	}


public:
	void SetNavigation(class CNavigation* Navigation)
	{
		m_Navigation = Navigation;
	}

	void AddWorkInfo(const Vector3& Start, const Vector3& Goal,
		class CSceneComponent* Component)
	{
		NavWorkInfo Info;

		Info.Start = Start;
		Info.Goal = Goal;
		Info.CallbackComponent = Component;

		m_WorkQueue.Push(Info);
	}

public:
	virtual void Run() = 0;
};

