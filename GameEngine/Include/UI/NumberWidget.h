#pragma once

#include "Widget.h"
#include "../Resource/NumberCBuffer.h"

class CNumberWidget :
	public CWidget
{
	friend class CViewport;
	friend class CScene;
	friend class CWidgetWindow;

protected:
	CNumberWidget();
	CNumberWidget(const CNumberWidget& widget);
	virtual ~CNumberWidget();

protected:
	CSharedPtr<CTexture>    m_Texture;
	int           m_Number;
	Vector2		m_NumberSize;	// 숫자 1개의 가로, 세로의 크기이다.
	std::vector<int>	m_vecNumber;
	CNumberCBuffer* m_NumberCBuffer;
	int		m_FrameCountX;
	int		m_FrameCountY;
	int		m_SelectYFrame;

public:
	void SetSelectYFrame(int Frame)
	{
		m_SelectYFrame = Frame;
	}

	void SetFrameCount(int x, int y)
	{
		m_FrameCountX = x;
		m_FrameCountY = y;
	}

	void SetNumberSize(float x, float y)
	{
		m_NumberSize = Vector2(x, y);
	}

	void SetNumberSize(const Vector2& Size)
	{
		m_NumberSize = Size;
	}

	void SetNumber(int Number)
	{
		m_Number = Number;
	}

	void AddNumber(int Number)
	{
		m_Number += Number;
	}

	void SetTexture(CTexture* Texture)
	{
		m_Texture = Texture;
	}

	void SetTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CNumberWidget* Clone();

private:
	void UpdateNumberTransform(int Index);
};

