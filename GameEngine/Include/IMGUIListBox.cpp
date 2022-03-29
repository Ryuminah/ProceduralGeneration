#include "IMGUIListBox.h"

CIMGUIListBox::CIMGUIListBox()	:
	m_SelectIndex(-1),
	m_Select(false),
	m_PageItemCount(3)
{
}

CIMGUIListBox::~CIMGUIListBox()
{
}

bool CIMGUIListBox::Init()
{
	return true;
}

void CIMGUIListBox::Render()
{
	if (m_Font)
		ImGui::PushFont(m_Font);

	ImVec2	Size = ImVec2(m_Size.x, m_PageItemCount * ImGui::GetTextLineHeightWithSpacing());


	ImGui::Text(m_Name);
	if (ImGui::BeginListBox(m_Name, Size))
	{
		size_t	Size = m_vecItem.size();

		for (size_t i = 0; i < Size; ++i)
		{
			m_Select = m_SelectIndex == i;

			if (ImGui::Selectable(m_vecItem[i].c_str(), m_Select))
			{
				if (m_SelectIndex != i && m_SelectCallback)
					m_SelectCallback((int)i, m_vecItem[i].c_str());

				m_SelectIndex = (int)i;
			}

			if (m_Select)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();
	}

	if (m_Font)
		ImGui::PopFont();
}
