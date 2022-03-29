#include "IMGUIText.h"

CIMGUIText::CIMGUIText()	:
	m_Text{},
	m_TextUTF8{},
	m_Align(true)
{
}

CIMGUIText::~CIMGUIText()
{
}

bool CIMGUIText::Init()
{
	return true;
}

void CIMGUIText::Render()
{
	if (m_Font)
		ImGui::PushFont(m_Font);

	if (m_Align)
		ImGui::AlignTextToFramePadding();

	ImGui::TextColored(m_Color, m_TextUTF8);


	if (m_Font)
		ImGui::PopFont();
}


