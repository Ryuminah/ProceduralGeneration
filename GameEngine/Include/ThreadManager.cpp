#include "ThreadManager.h"

DEFINITION_SINGLE(CThreadManager)

CThreadManager::CThreadManager()
{
}

CThreadManager::~CThreadManager()
{
	{
		auto	iter = m_mapThread.begin();
		auto	iterEnd = m_mapThread.end();

		for (; iter != iterEnd; ++iter)
		{
			iter->second->DestroyThread();
			SAFE_DELETE(iter->second);
		}

		m_mapThread.clear();
	}

	{
		auto	iter = m_mapCriticalSection.begin();
		auto	iterEnd = m_mapCriticalSection.end();

		for (; iter != iterEnd; ++iter)
		{
			DeleteCriticalSection(iter->second);
			SAFE_DELETE(iter->second);
		}

		m_mapCriticalSection.clear();
	}
}

bool CThreadManager::Init()
{
	return true;
}

bool CThreadManager::CreateCriticalSection(const std::string& Name)
{
	CRITICAL_SECTION* Crt = FindCriticalSection(Name);

	if (Crt)
		return false;

	Crt = new CRITICAL_SECTION;

	InitializeCriticalSection(Crt);

	m_mapCriticalSection.insert(std::make_pair(Name, Crt));

	return true;
}

CThread* CThreadManager::FindThread(const std::string& Name)
{
	auto	iter = m_mapThread.find(Name);

	if (iter == m_mapThread.end())
		return nullptr;

	return iter->second;
}

CRITICAL_SECTION* CThreadManager::FindCriticalSection(const std::string& Name)
{
	auto	iter = m_mapCriticalSection.find(Name);

	if (iter == m_mapCriticalSection.end())
		return nullptr;

	return iter->second;
}
