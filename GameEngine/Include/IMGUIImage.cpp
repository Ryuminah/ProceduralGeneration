#include "IMGUIImage.h"
#include "Resource/ResourceManager.h"

CIMGUIImage::CIMGUIImage()
{
}

CIMGUIImage::~CIMGUIImage()
{
}

void CIMGUIImage::SetTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);
}

void CIMGUIImage::SetTextureFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath);

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);
}

void CIMGUIImage::SetTexture(CTexture* Texture)
{
	m_Texture = Texture;
}

bool CIMGUIImage::Init()
{
	m_UVStart = ImVec2(0.f, 0.f);
	m_UVEnd = ImVec2(1.f, 1.f);

	m_Tint = ImVec4(1.f, 1.f, 1.f, 1.f);
	m_BorderColor = ImVec4(0.f, 0.f, 0.f, 0.f);

	return true;
}

void CIMGUIImage::Render()
{
	if (m_Texture)
		ImGui::Image(m_Texture->GetResource(), m_Size, m_UVStart, m_UVEnd, m_Tint, m_BorderColor);
}
