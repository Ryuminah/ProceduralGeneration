#pragma once

#include "Thread.h"

class CLoadingThread :
    public CThread
{
public:
	CLoadingThread();
	~CLoadingThread();

public:
	virtual void Run();
};

