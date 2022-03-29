#include "IMGUISameLine.h"

CIMGUISameLine::CIMGUISameLine() :
	m_StartX(0.f),
	m_Spacing(-1.f)
{
}

CIMGUISameLine::~CIMGUISameLine()
{
}

bool CIMGUISameLine::Init()
{
	return true;
}

void CIMGUISameLine::Render()
{
	ImGui::SameLine(m_StartX, m_Spacing);
}

