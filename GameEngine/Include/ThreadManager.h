#pragma once

#include "Thread.h"

class CThreadManager
{
private:
	std::unordered_map<std::string, CThread*>	m_mapThread;
	std::unordered_map<std::string, CRITICAL_SECTION*>	m_mapCriticalSection;

public:
	bool Init();
	bool CreateCriticalSection(const std::string& Name);
	CThread* FindThread(const std::string& Name);
	CRITICAL_SECTION* FindCriticalSection(const std::string& Name);

public:
	template <typename T>
	T* CreateThread(const std::string& Name)
	{
		T* Thread = (T*)FindThread(Name);

		if (Thread)
			return nullptr;

		Thread = new T;

		Thread->Create();

		m_mapThread.insert(std::make_pair(Name, Thread));

		return Thread;
	}

	DECLARE_SINGLE(CThreadManager)
};

