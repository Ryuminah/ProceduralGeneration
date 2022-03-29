#pragma once

#include "Sync.h"

template <typename T, int SIZE = 200>
class CQueue
{
public:
	CQueue()
	{
		m_Push = 0;
		m_Pop = 0;
		m_Size = 0;
		InitializeCriticalSection(&m_Crt);
	}

	~CQueue()
	{
		LeaveCriticalSection(&m_Crt);
	}

private:
	T	m_Data[SIZE + 1];
	int	m_Push;
	int	m_Pop;
	int	m_Size;
	CRITICAL_SECTION	m_Crt;

public:
	bool Push(const T& data)
	{
		CSync	sync(&m_Crt);

		if (Full())
			return false;

		int	PushIndex = (m_Push + 1) % (SIZE + 1);

		m_Data[PushIndex] = data;
		m_Push = PushIndex;

		++m_Size;

		return true;
	}

	bool Pop(T& data)
	{
		CSync	sync(&m_Crt);

		if (Empty())
			return false;

		int	PopIndex = (m_Pop + 1) % (SIZE + 1);

		data = m_Data[PopIndex];
		m_Pop = PopIndex;

		--m_Size;

		return true;
	}

	bool Empty()
	{
		CSync	sync(&m_Crt);

		return m_Size == 0;
	}

	bool Full()
	{
		CSync	sync(&m_Crt);

		return m_Size == SIZE;
	}

	int Size()	const
	{
		return m_Size;
	}
};

