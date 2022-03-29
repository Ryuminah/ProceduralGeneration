#include "IMGUITextInput.h"
#include "imgui_internal.h"

CIMGUITextInput::CIMGUITextInput() :
	m_Text{},
	m_TextUTF8{},
	m_TextMultibyte{},
	m_HintText{},
	m_HintTextUTF8{},
	m_HintTextMultibyte{},
	m_Align(true),
	m_MultiLine(false),
	m_Flag(0),
	m_ValueInt(0),
	m_ValueFloat(0.f),
	m_NumberInt(false),
	m_NumberFloat(false)
{
}

CIMGUITextInput::~CIMGUITextInput()
{
}

bool CIMGUITextInput::Init()
{
	return true;
}

void CIMGUITextInput::Render()
{
	if (m_Font)
		ImGui::PushFont(m_Font);

	//if (m_Align)
	//	ImGui::AlignTextToFramePadding();

	bool	Input = false;

	ImGui::PushItemWidth(m_Size.x);

	//ImGui::TextColored(m_Color, m_TextUTF8);
	//ImGui::InputText
	if (m_MultiLine)
	{
		Input = ImGui::InputTextMultiline(m_Name, m_TextMultibyte, 1024, m_Size,
			m_Flag);
	}

	else
	{
		if (m_NumberInt)
		{
			Input = ImGui::InputInt(m_Name, &m_ValueInt, 1, m_Flag);
		}

		else if (m_NumberFloat)
		{
			Input = ImGui::InputFloat(m_Name, &m_ValueFloat, 0.f, 0.f, "%.3f", m_Flag);

			int a = 10;
		}

		else if (strlen(m_HintTextMultibyte) > 0)
		{
			Input = ImGui::InputTextWithHint(m_Name, m_HintTextUTF8, m_TextMultibyte,
				1024, m_Flag);
		}

		else
		{
			Input = ImGui::InputTextEx(m_Name, "", m_TextMultibyte, 1024, m_Size, m_Flag);
		}
	}
	
	if (Input)
	{
		if (!m_NumberInt && !m_NumberFloat)
		{
			int	Length = MultiByteToWideChar(CP_ACP, 0, m_TextMultibyte, -1, 0, 0);
			MultiByteToWideChar(CP_ACP, 0, m_TextMultibyte, -1, m_Text, Length);

			// UTF8로 변환한다.
			Length = WideCharToMultiByte(CP_UTF8, 0, m_Text, -1, 0, 0, 0, 0);
			WideCharToMultiByte(CP_UTF8, 0, m_Text, -1, m_TextUTF8, Length, 0, 0);
		}

		if (m_InputCallback)
			m_InputCallback();
	}


	if (m_Font)
		ImGui::PopFont();
}
