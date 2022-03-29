#pragma once
#include "IMGUIWindow.h"
class CLogWindow :
    public CIMGUIWindow
{
public:
	CLogWindow();
	virtual ~CLogWindow();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

