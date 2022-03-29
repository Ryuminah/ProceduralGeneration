#pragma once
#include "IMGUIWidget.h"

struct ConsoleItem
{
	char	Text[1024];
	ImVec4	Color;

	ConsoleItem() :
		Text{},
		Color(1.f, 1.f, 1.f, 1.f)
	{
	}
};

class CIMGUIConsole :
    public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIConsole();
	virtual ~CIMGUIConsole();

protected:
	std::list<ConsoleItem>	m_HistoryList;
	int		m_MaxHistory;
	bool	m_AutoScroll;

public:
	void SetMaxHistory(int Count);
	void AutoScroll(bool Scroll)
	{
		m_AutoScroll = Scroll;
	}
	void Clear()
	{
		m_HistoryList.clear();
	}

	void AddText(const char* Text, const unsigned char r = 255, const unsigned char g = 255,
		const unsigned char b = 255, const unsigned char a = 255);
	void AddTextImgui(const char* Text, const ImVec4& Color = ImVec4(1.f, 1.f, 1.f, 1.f));
	void DeleteText();

public:
	virtual bool Init();
	virtual void Render();
};

