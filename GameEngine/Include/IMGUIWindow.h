#pragma once

#include "GameEngine.h"

class CIMGUIWindow
{
public:
	CIMGUIWindow();
	virtual ~CIMGUIWindow();

private:
	char	m_Name[256];
	bool	m_Open;
	int		m_WindowFlag;
	std::vector<class CIMGUIWidget*>	m_vecWidget;
	// 기본 폰트 지정
	ImFont* m_Font;

private:
	char	m_PopupTitle[256];
	ImFont* m_PopupFont;
	bool	m_ModalPopup;
	std::vector<class CIMGUIWidget*>	m_PopupWidget;

public:
	ImFont* GetFont()	const
	{
		return m_Font;
	}

	ImFont* GetPopupFont()	const
	{
		return m_PopupFont;
	}

public:
	void SetName(const std::string& Name)
	{
		strcpy_s(m_Name, Name.c_str());
	}

	void AddWindowFlag(ImGuiWindowFlags_ Flag)
	{
		m_WindowFlag |= Flag;
	}

	void EnableModalPopup()
	{
		m_ModalPopup = true;
	}

	void SetFont(const std::string& Name);
	void SetPopupFont(const std::string& Name);
	void SetPopupTitle(const std::string& PopupTitle);
	void Open();
	void Close();
	void ClosePopup();
	class CIMGUIWidget* FindWidget(const std::string& Name);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	template <typename T>
	T* AddWidget(const std::string& Name, float SizeX = 100.f, float SizeY = 100.f)
	{
		T* Widget = new T;

		Widget->SetName(Name);
		Widget->m_Owner = this;
		Widget->SetSize(SizeX, SizeY);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		m_vecWidget.push_back(Widget);

		return Widget;
	}

	template <typename T>
	T* AddPopupWidget(const std::string& Name, float SizeX = 100.f, float SizeY = 100.f)
	{
		T* Widget = new T;

		Widget->SetName(Name);
		Widget->m_Owner = this;
		Widget->SetSize(SizeX, SizeY);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		m_PopupWidget.push_back(Widget);

		return Widget;
	}
};

