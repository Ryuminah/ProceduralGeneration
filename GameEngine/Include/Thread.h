#pragma once

#include "GameEngine.h"

class CThread
{
	friend class CThreadManager;

protected:
	CThread();
	virtual ~CThread() = 0;

private:
	HANDLE	m_hThread;
	HANDLE	m_hStart;
	bool	m_Loop;

public:
	bool Create();
	virtual void Run() = 0;

public:
	void Wait();
	void Start();
	void SetLoop(bool Loop)
	{
		m_Loop = Loop;
	}

	void DestroyThread();

public:
	static unsigned int __stdcall ThreadFunc(void* Arg);
};

