#include "IMGUIManager.h"
#include "Device.h"
#include "IMGUIWindow.h"
#include "PathManager.h"
#include "IMGUIButton.h"

DEFINITION_SINGLE(CIMGUIManager)

CIMGUIManager::CIMGUIManager()	:
	m_Context(nullptr)
{
}

CIMGUIManager::~CIMGUIManager()
{
	{
		auto	iter = m_mapWindow.begin();
		auto	iterEnd = m_mapWindow.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE(iter->second);
		}
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool CIMGUIManager::Init(HWND hWnd)
{
	//IMGUI_CHECKVERSION();
	//ImGui_ImplWin32_EnableDpiAwareness();

	m_Context = ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Multi-Viewports 활성화
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;

	ImGui::StyleColorsDark();

	// backend Setup
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(DEVICE, CONTEXT);

	AddFont("DefaultFont", "NotoSansKR-Regular.otf", 15.f, true);

	return true;
}

void CIMGUIManager::Update(float DeltaTime)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	auto	iter = m_mapWindow.begin();
	auto	iterEnd = m_mapWindow.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->Update(DeltaTime);
	}


	// 위에서 IMGUI에서 제공한 gui 들을 그려냈다면 실제 화면에 그려낸것이 아니라
	// IMGUI 버퍼에 그려진 정보들이 들어가 있다.
	// 나중에 출력할때 그 정보들을 화면에 렌더링하는 방식이다.
	ImGui::Render();
}

void CIMGUIManager::Render()
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

CIMGUIWindow* CIMGUIManager::FindIMGUIWindow(const std::string& Name)
{
	auto	iter = m_mapWindow.find(Name);

	if (iter == m_mapWindow.end())
		return nullptr;

	return iter->second;
}

bool CIMGUIManager::AddFont(const std::string& Name, const char* FileName,
	float Size, bool Korea, int OverH, int OverV, float Spacing,
	const std::string& PathName)
{
	char	FullPath[MAX_PATH] = {};

	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	if (Path)
		strcpy_s(FullPath, Path->pPathMultibyte);

	strcat_s(FullPath, FileName);

	return AddFontFullPath(Name, FullPath, Size, Korea, OverH, OverV, Spacing);
}

bool CIMGUIManager::AddFontFullPath(const std::string& Name,
	const char* FullPath, float Size, bool Korea, int OverH, int OverV,
	float Spacing)
{
	ImFont* Font = FindFont(Name);

	if (Font)
		return false;

	ImGuiIO& io = ImGui::GetIO();

	ImFontConfig	config;

	config.OversampleH = OverH;
	config.OversampleV = OverV;
	config.GlyphExtraSpacing.x = Spacing;
	config.PixelSnapH = 1;

	if (!Korea)
		Font = io.Fonts->AddFontFromFileTTF(FullPath, Size, &config, io.Fonts->GetGlyphRangesDefault());

	else
		Font = io.Fonts->AddFontFromFileTTF(FullPath, Size, &config, io.Fonts->GetGlyphRangesKorean());

	m_mapFont.insert(std::make_pair(Name, Font));

	return true;
}

ImFont* CIMGUIManager::FindFont(const std::string& Name)
{
	auto	iter = m_mapFont.find(Name);

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}
