#include "IMGUILabel.h"

CIMGUILabel::CIMGUILabel()	:
	m_Text{}
{
	SetColor(0.3f, 0.3f, 0.3f);
	m_Align = ImVec2(0.f, 0.f);
}

CIMGUILabel::~CIMGUILabel()
{
}

bool CIMGUILabel::Init()
{
	return true;
}

void CIMGUILabel::Render()
{
	if (m_Font)
		ImGui::PushFont(m_Font);

	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, m_Align);

	ImGui::PushStyleColor(ImGuiCol_Button, m_Color.Value);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_Color.Value);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_Color.Value);

	ImGui::Button(m_Text, m_Size);

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();

	if (m_Font)
		ImGui::PopFont();
}


