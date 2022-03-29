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

	void SetAmbientColor(const Vector4& Color)
	{
		m_BufferData.AmbientColor = Color;
	}

	void SetSpecularColor(const Vector4& Color)
	{
		m_BufferData.SpecularColor = Color;
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

	void BumpEnable(bool Enable)
	{
		m_BufferData.BumpEnable = Enable ? 1 : 0;
	}

	void Animation3DEnable(bool Enable)
	{
		m_BufferData.Animation3DEnable = Enable ? 1 : 0;
	}

	void SpecularTexEnable(bool Enable)
	{
		m_BufferData.SpecularTex = Enable ? 1 : 0;
	}

	void EmissiveTexEnable(bool Enable)
	{
		m_BufferData.EmissiveTex = Enable ? 1 : 0;
	}

	void ReceiveDecal(bool Enable)
	{
		m_BufferData.ReceiveDecal = Enable ? 1 : 0;
	}
};

