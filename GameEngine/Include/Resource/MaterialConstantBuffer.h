#pragma once

#include "ConstantBufferData.h"

class CMaterialConstantBuffer :
	public CConstantBufferData
{
public:
	CMaterialConstantBuffer();
	CMaterialConstantBuffer(const CMaterialConstantBuffer& buffer);
	virtual ~CMaterialConstantBuffer();

protected:
	MaterialCBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CMaterialConstantBuffer* Clone();

public:
	void SetBaseColor(const Vector4& Color)
	{
		m_BufferData.BaseColor = Color;
	}

	void SetEmissiveColor(const Vector4& Color)
	{
		m_BufferData.EmissiveColor = Color;
	}

	void SetOpacity(float Opacity)
	{
		m_BufferData.Opacity = Opacity;
	}

	void PaperBurnEnable(bool Enable)
	{
		m_BufferData.PaperBurn = Enable ? 1 : 0;
	}

	void DistortionEnable(bool Enable)
	{
		m_BufferData.Distortion = Enable ? 1 : 0;
	}
};

