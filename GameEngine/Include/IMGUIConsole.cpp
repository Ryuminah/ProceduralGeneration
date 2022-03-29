#include "IMGUIConsole.h"

CIMGUIConsole::CIMGUIConsole()	:
	m_MaxHistory(1000),
	m_AutoScroll(true)
{
}

CIMGUIConsole::~CIMGUIConsole()
{
}

void CIMGUIConsole::SetMaxHistory(int Count)
{
	m_MaxHistory = Count;

	if ((int)m_HistoryList.size() > m_MaxHistory)
	{
		int	PopCount = (int)m_HistoryList.size() - m_MaxHistory;

		for (int i = 0; i < PopCount; ++i)
		{
			m_HistoryList.pop_front();
		}
	}
}

void CIMGUIConsole::AddText(const char* Text, const unsigned char r, const unsigned char g,
    const unsigned char b, const unsigned char a)
{
    AddTextImgui(Text, ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f));
}

void CIMGUIConsole::AddTextImgui(const char* Text, const ImVec4& Color)
{
    ConsoleItem Item;

    strcpy_s(Item.Text, Text);
    Item.Color = Color;

    m_HistoryList.push_back(Item);

    if (m_HistoryList.size() > m_MaxHistory)
        m_HistoryList.pop_front();
}

void CIMGUIConsole::DeleteText()
{
    if(!m_HistoryList.empty())
        m_HistoryList.pop_front();
}

bool CIMGUIConsole::Init()
{
	return true;
}

void CIMGUIConsole::Render()
{
	if (m_Font)
		ImGui::PushFont(m_Font);

	auto	iter = m_HistoryList.begin();
	auto	iterEnd = m_HistoryList.end();

	for (; iter != iterEnd; ++iter)
	{
        ImGui::PushStyleColor(ImGuiCol_Text, (*iter).Color);
        ImGui::TextUnformatted((*iter).Text);
        ImGui::PopStyleColor();
	}

    if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.f);


	if (m_Font)
		ImGui::PopFont();
}





/*
enum ImGuiCol_
{
    ImGuiCol_Text,
    ImGuiCol_TextDisabled,
    ImGuiCol_WindowBg,              // Background of normal windows
    ImGuiCol_ChildBg,               // Background of child windows
    ImGuiCol_PopupBg,               // Background of popups, menus, tooltips windows
    ImGuiCol_Border,
    ImGuiCol_BorderShadow,
    ImGuiCol_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
    ImGuiCol_FrameBgHovered,
    ImGuiCol_FrameBgActive,
    ImGuiCol_TitleBg,
    ImGuiCol_TitleBgActive,
    ImGuiCol_TitleBgCollapsed,
    ImGuiCol_MenuBarBg,
    ImGuiCol_ScrollbarBg,
    ImGuiCol_ScrollbarGrab,
    ImGuiCol_ScrollbarGrabHovered,
    ImGuiCol_ScrollbarGrabActive,
    ImGuiCol_CheckMark,
    ImGuiCol_SliderGrab,
    ImGuiCol_SliderGrabActive,
    ImGuiCol_Button,
    ImGuiCol_ButtonHovered,
    ImGuiCol_ButtonActive,
    ImGuiCol_Header,                // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
    ImGuiCol_HeaderHovered,
    ImGuiCol_HeaderActive,
    ImGuiCol_Separator,
    ImGuiCol_SeparatorHovered,
    ImGuiCol_SeparatorActive,
    ImGuiCol_ResizeGrip,
    ImGuiCol_ResizeGripHovered,
    ImGuiCol_ResizeGripActive,
    ImGuiCol_Tab,
    ImGuiCol_TabHovered,
    ImGuiCol_TabActive,
    ImGuiCol_TabUnfocused,
    ImGuiCol_TabUnfocusedActive,
    ImGuiCol_DockingPreview,        // Preview overlay color when about to docking something
    ImGuiCol_DockingEmptyBg,        // Background color for empty node (e.g. CentralNode with no window docked into it)
    ImGuiCol_PlotLines,
    ImGuiCol_PlotLinesHovered,
    ImGuiCol_PlotHistogram,
    ImGuiCol_PlotHistogramHovered,
    ImGuiCol_TableHeaderBg,         // Table header background
    ImGuiCol_TableBorderStrong,     // Table outer and header borders (prefer using Alpha=1.0 here)
    ImGuiCol_TableBorderLight,      // Table inner borders (prefer using Alpha=1.0 here)
    ImGuiCol_TableRowBg,            // Table row background (even rows)
    ImGuiCol_TableRowBgAlt,         // Table row background (odd rows)
    ImGuiCol_TextSelectedBg,
    ImGuiCol_DragDropTarget,
    ImGuiCol_NavHighlight,          // Gamepad/keyboard: current highlighted item
    ImGuiCol_NavWindowingHighlight, // Highlight window when using CTRL+TAB
    ImGuiCol_NavWindowingDimBg,     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
    ImGuiCol_ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active
    ImGuiCol_COUNT
};
*/