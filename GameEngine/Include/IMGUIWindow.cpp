#include "IMGUIWindow.h"
#include "IMGUIWidget.h"
#include "IMGUIManager.h"

CIMGUIWindow::CIMGUIWindow()    :
	m_Name{},
	m_Open(true),
	m_WindowFlag(0),
	m_Font(nullptr),
	m_PopupTitle{},
	m_PopupFont(nullptr),
	m_ModalPopup(false)
{
}

CIMGUIWindow::~CIMGUIWindow()
{
	size_t  Size = m_vecWidget.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecWidget[i]);
	}
}

void CIMGUIWindow::SetFont(const std::string& Name)
{
	m_Font = CIMGUIManager::GetInst()->FindFont(Name);
}

void CIMGUIWindow::SetPopupFont(const std::string& Name)
{
	m_PopupFont = CIMGUIManager::GetInst()->FindFont(Name);
}

void CIMGUIWindow::SetPopupTitle(const std::string& PopupTitle)
{
	strcpy_s(m_PopupTitle, PopupTitle.c_str());
}

void CIMGUIWindow::Open()
{
	m_Open = true;
}

void CIMGUIWindow::Close()
{
	m_Open = false;
}

void CIMGUIWindow::ClosePopup()
{
	ImGui::CloseCurrentPopup();

	size_t  Size = m_PopupWidget.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_PopupWidget[i]);
	}

	m_PopupWidget.clear();
}

CIMGUIWidget* CIMGUIWindow::FindWidget(const std::string& Name)
{
	auto	iter = m_vecWidget.begin();
	auto	iterEnd = m_vecWidget.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return *iter;
	}

	return nullptr;
}

bool CIMGUIWindow::Init()
{
	SetFont("DefaultFont");
	SetPopupFont("DefaultFont");

	return true;
}

void CIMGUIWindow::Update(float DeltaTime)
{
	//ImVec2  Pos(500, 300);
	//ImGui::SetWindowPos(m_Name, Pos);
	if (m_Open)
	{
		//ImGui::SetWindowSize(m_Size, ImGuiCond_Once);
		if (!ImGui::Begin(m_Name, &m_Open, m_WindowFlag))
			m_Open = false;

		if (m_Font)
			ImGui::PushFont(m_Font);

		size_t  WidgetSize = m_vecWidget.size();

		for (size_t i = 0; i < WidgetSize; ++i)
		{
			m_vecWidget[i]->Render();
		}

		if (m_Font)
			ImGui::PopFont();

		if (!m_PopupWidget.empty())
		{
			ImGui::OpenPopup(m_PopupTitle);

			if (!m_ModalPopup)
			{
				if (ImGui::BeginPopup(m_PopupTitle))
				{
					if (m_PopupFont)
						ImGui::PushFont(m_PopupFont);

					// ÆË¾÷¿ë À§Á¬À» ±×¸°´Ù.
					size_t  Size = m_PopupWidget.size();
					for (size_t i = 0; i < Size; ++i)
					{
						m_PopupWidget[i]->Render();
					}


					if (m_PopupFont)
						ImGui::PopFont();

					ImGui::EndPopup();
				}
			}

			else
			{
				if (ImGui::BeginPopupModal(m_PopupTitle))
				{
					if (m_PopupFont)
						ImGui::PushFont(m_PopupFont);

					// ÆË¾÷¿ë À§Á¬À» ±×¸°´Ù.
					size_t  Size = m_PopupWidget.size();
					for (size_t i = 0; i < Size; ++i)
					{
						m_PopupWidget[i]->Render();
					}


					if (m_PopupFont)
						ImGui::PopFont();

					ImGui::EndPopup();
				}
			}
		}

		ImGui::End();
	}
}
