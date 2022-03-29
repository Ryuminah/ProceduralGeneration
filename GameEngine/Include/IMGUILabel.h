#pragma once
#include "IMGUIWidget.h"
class CIMGUILabel :
    public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUILabel();
	virtual ~CIMGUILabel();

protected:
	char	m_Text[1024];
	ImVec2	m_Align;

public:
	void SetText(const char* Text)
	{
		strcpy_s(m_Text, Text);
	}

	void AddText(const char* Text)
	{
		strcat_s(m_Text, Text);
	}

	void SetAlign(float x, float y)
	{
		m_Align = ImVec2(x, y);
	}

public:
	virtual bool Init();
	virtual void Render();
};

