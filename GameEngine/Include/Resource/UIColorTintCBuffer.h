#pragma once
#include "ConstantBufferData.h"
class CUIColorTintCBuffer :
    public CConstantBufferData
{
public:
	CUIColorTintCBuffer();
	CUIColorTintCBuffer(const CUIColorTintCBuffer& buffer);
	virtual ~CUIColorTintCBuffer();

protected:
	UIColorTintCBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CUIColorTintCBuffer* Clone();

public:
	void SetTint(float r, float g, float b, float a)
	{
		m_BufferData.Tint.x = r;
		m_BufferData.Tint.y = g;
		m_BufferData.Tint.z = b;
		m_BufferData.Tint.w = a;
	}

	void SetTint(const Vector4& Color)
	{
		m_BufferData.Tint = Color;
	}
};

