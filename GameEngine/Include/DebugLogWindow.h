#pragma once
#include "IMGUIWindow.h"
class CDebugLogWindow :
    public CIMGUIWindow
{
public:
	CDebugLogWindow();
	virtual ~CDebugLogWindow();

private:
	class CIMGUIConsole* m_Console;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	void ClearButton();
	void AddLog(const char* Text);
};

