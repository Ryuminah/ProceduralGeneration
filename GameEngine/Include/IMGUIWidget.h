#pragma once

#include "GameEngine.h"

class CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIWidget();
	virtual ~CIMGUIWidget();

protected:
	char	m_Name[256];
	wchar_t	m_UnicodeName[256];
	class CIMGUIWindow* m_Owner;
	ImVec2	m_Size;
	ImColor	m_Color;
	ImFont* m_Font;

public:
	const char* GetName()	const
	{
		return m_Name;
	}

public:
	void SetName(const std::string& Name)
	{
		// 변환할 문자열 수를 얻어온다.
		int	Length = MultiByteToWideChar(CP_ACP, 0, Name.c_str(), -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, Name.c_str(), -1, m_UnicodeName, Length);

		// UTF8로 변환한다.
		Length = WideCharToMultiByte(CP_UTF8, 0, m_UnicodeName, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_UTF8, 0, m_UnicodeName, -1, m_Name, Length, 0, 0);
	}

	void SetSize(float x, float y)
	{
		m_Size.x = x;
		m_Size.y = y;
	}

	void SetSize(const ImVec2& Size)
	{
		m_Size = Size;
	}

	void SetColor(float r, float g, float b, float a = 1.f)
	{
		m_Color.Value.x = r;
		m_Color.Value.y = g;
		m_Color.Value.z = b;
		m_Color.Value.w = a;
	}

	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
	{
		m_Color.Value.x = r / 255.f;
		m_Color.Value.y = g / 255.f;
		m_Color.Value.z = b / 255.f;
		m_Color.Value.w = a / 255.f;
	}

	void SetFont(const std::string& Name);

public:
	virtual bool Init();
	virtual void Render();
};

