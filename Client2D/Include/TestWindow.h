#pragma once

#include "IMGUIWindow.h"

class CTestWindow :
    public CIMGUIWindow
{
public:
	CTestWindow();
	virtual ~CTestWindow();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	void TestButtonClick();
};

