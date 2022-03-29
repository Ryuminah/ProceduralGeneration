#include "IMGUIButton.h"

CIMGUIButton::CIMGUIButton()    :
	m_Click(false)
{
}

CIMGUIButton::~CIMGUIButton()
{
}

bool CIMGUIButton::Init()
{
	return true;
}

void CIMGUIButton::Render()
{
	if (m_Font)
		ImGui::PushFont(m_Font);

	m_Click = ImGui::Button(m_Name, m_Size);

	if (m_Font)
		ImGui::PopFont();

	if (m_Click && m_ClickCallback)
		m_ClickCallback();
}
