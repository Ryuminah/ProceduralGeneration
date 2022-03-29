#pragma once
#include "IMGUIWidget.h"
class CIMGUISameLine :
    public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUISameLine();
	virtual ~CIMGUISameLine();

protected:
	float	m_StartX;
	float	m_Spacing;

public:
	void SetStartX(float StartX)
	{
		m_StartX = StartX;
	}

	void SetSpacing(float Spacing)
	{
		m_Spacing = Spacing;
	}

public:
	virtual bool Init();
	virtual void Render();
};

