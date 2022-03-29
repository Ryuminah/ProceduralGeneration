#pragma once
#include "IMGUIWidget.h"
#include "Resource/Texture.h"
class CIMGUIImage :
    public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIImage();
	virtual ~CIMGUIImage();

protected:
	CSharedPtr<CTexture> m_Texture;
	ImVec2		m_UVStart;
	ImVec2		m_UVEnd;
	ImVec4		m_Tint;
	ImVec4		m_BorderColor;
	
public:
	void SetStartUV(float x, float y)
	{
		m_UVStart = ImVec2(x, y);
	}

	void SetEndUV(float x, float y)
	{
		m_UVEnd = ImVec2(x, y);
	}

	void SetTint(float r, float g, float b, float a)
	{
		m_Tint = ImVec4(r, g, b, a);
	}

	void SetBorderColor(float r, float g, float b, float a)
	{
		m_BorderColor = ImVec4(r, g, b, a);
	}

public:
	void SetTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	void SetTexture(class CTexture* Texture);

public:
	virtual bool Init();
	virtual void Render();
};

