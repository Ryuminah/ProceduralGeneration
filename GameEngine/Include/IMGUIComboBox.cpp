#include "IMGUIComboBox.h"

CIMGUIComboBox::CIMGUIComboBox() :
	m_SelectIndex(-1),
	m_Select(false),
	m_Flag(0)
{
}

CIMGUIComboBox::~CIMGUIComboBox()
{
}

bool CIMGUIComboBox::Init()
{
	m_PrevViewName = m_Name;

	return true;
}

void CIMGUIComboBox::Render()
{
	if (m_Font)
		ImGui::PushFont(m_Font);

	ImGui::PushItemWidth(m_Size.x);

	if (ImGui::BeginCombo(m_Name, m_PrevViewName.c_str(), m_Flag))
	{
		size_t	Size = m_vecItem.size();

		for (size_t i = 0; i < Size; ++i)
		{
			m_Select = m_SelectIndex == i;

			if (ImGui::Selectable(m_vecItem[i].c_str(), m_Select))
			{
				if (m_SelectIndex != i && m_SelectCallback)
					m_SelectCallback((int)i, m_vecItem[i].c_str());

				m_PrevViewName = m_vecItem[i];

				m_SelectIndex = (int)i;
			}

			if (m_Select)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	if (m_Font)
		ImGui::PopFont();
}

