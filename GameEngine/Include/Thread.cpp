#include "Thread.h"

CThread::CThread()	:
	m_hThread(0),
	m_hStart(0),
	m_Loop(true)
{
}

CThread::~CThread()
{
	DestroyThread();
}

bool CThread::Create()
{
	m_hStart = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, CThread::ThreadFunc,
		this, 0, nullptr);

	return true;
}

void CThread::Wait()
{
	WaitForSingleObject(m_hStart, INFINITE);
}

void CThread::Start()
{
	SetEvent(m_hStart);
}

void CThread::DestroyThread()
{
	m_Loop = false;

	if (m_hStart)
	{
		CloseHandle(m_hStart);
		m_hStart = 0;
	}

	if (m_hThread)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = 0;
	}
}

unsigned int __stdcall CThread::ThreadFunc(void* Arg)
{
	CThread* Thread = (CThread*)Arg;

	Thread->Wait();

	do
	{
		Thread->Run();
	} while (Thread->m_Loop);

	return 0;
}
