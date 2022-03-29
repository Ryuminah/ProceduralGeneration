#pragma once
#include "IMGUIWidget.h"
class CIMGUITextInput :
    public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUITextInput();
	virtual ~CIMGUITextInput();

protected:
	wchar_t	m_Text[1024];
	char	m_TextUTF8[1024];
	char	m_TextMultibyte[1024];

	wchar_t	m_HintText[1024];
	char	m_HintTextUTF8[1024];
	char	m_HintTextMultibyte[1024];
	bool	m_Align;
	bool	m_MultiLine;
	ImGuiInputTextFlags	m_Flag;

	bool	m_NumberInt;
	bool	m_NumberFloat;

	int		m_ValueInt;
	float	m_ValueFloat;

	std::function<void()>	m_InputCallback;

public:
	int GetValueInt()	const
	{
		return m_ValueInt;
	}

	float GetValueFloat()	const
	{
		return m_ValueFloat;
	}

	const wchar_t* GetText()	const
	{
		return m_Text;
	}

	const char* GetTextUtf8()	const
	{
		return m_TextUTF8;
	}

	const char* GetTextMultibyte()	const
	{
		return m_TextMultibyte;
	}

public:
	void SetHintText(const char* Text)
	{
		strcpy_s(m_HintTextMultibyte, Text);
		// 변환할 문자열 수를 얻어온다.
		int	Length = MultiByteToWideChar(CP_ACP, 0, Text, -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, Text, -1, m_HintText, Length);

		// UTF8로 변환한다.
		Length = WideCharToMultiByte(CP_UTF8, 0, m_HintText, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_UTF8, 0, m_HintText, -1, m_HintTextUTF8, Length, 0, 0);
	}

	void AddHintText(const char* Text)
	{
		strcat_s(m_HintTextMultibyte, Text);
		// 변환할 문자열 수를 얻어온다.
		wchar_t	WideText[1024] = {};
		int	Length = MultiByteToWideChar(CP_ACP, 0, Text, -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, Text, -1, WideText, Length);
		lstrcat(m_HintText, WideText);

		// UTF8로 변환한다.
		Length = WideCharToMultiByte(CP_UTF8, 0, m_HintText, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_UTF8, 0, m_HintText, -1, m_HintTextUTF8, Length, 0, 0);
	}

	void SetText(const char* Text)
	{
		strcpy_s(m_TextMultibyte, Text);

		// 변환할 문자열 수를 얻어온다.
		int	Length = MultiByteToWideChar(CP_ACP, 0, Text, -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, Text, -1, m_Text, Length);

		// UTF8로 변환한다.
		Length = WideCharToMultiByte(CP_UTF8, 0, m_Text, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_UTF8, 0, m_Text, -1, m_TextUTF8, Length, 0, 0);
	}

	void SetInt(int Value)
	{
		m_ValueInt = Value;
	}

	void SetFloat(float Value)
	{
		m_ValueFloat = Value;
	}

	void SetAlign(bool Align)
	{
		m_Align = Align;
	}

	void SetMultiLine(bool MultiLine)
	{
		m_MultiLine = MultiLine;
	}

	void AddFlag(ImGuiInputTextFlags_ Flag)
	{
		m_Flag |= Flag;
	}

	void ReadOnly(bool Enable)
	{
		if (Enable)
			m_Flag |= ImGuiInputTextFlags_ReadOnly;

		else if (m_Flag & ImGuiInputTextFlags_ReadOnly)
			m_Flag ^= ImGuiInputTextFlags_ReadOnly;
	}

	void EnablePassword(bool Enable)
	{
		if (Enable)
			m_Flag |= ImGuiInputTextFlags_Password;

		else if (m_Flag & ImGuiInputTextFlags_Password)
			m_Flag ^= ImGuiInputTextFlags_Password;
	}

	void SetNumberInt(bool NumberInt)
	{
		m_NumberInt = NumberInt;
	}

	void SetNumberFloat(bool NumberFloat)
	{
		m_NumberFloat = NumberFloat;
	}

public:
	virtual bool Init();
	virtual void Render();

public:
	template <typename T>
	void SetInputCallback(T* Obj, void(T::* Func)())
	{
		m_InputCallback = std::bind(Func, Obj);
	}
};

