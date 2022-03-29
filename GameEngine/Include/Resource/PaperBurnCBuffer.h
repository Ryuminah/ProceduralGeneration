#pragma once
#include "ConstantBufferData.h"
class CPaperBurnCBuffer :
    public CConstantBufferData
{
public:
	CPaperBurnCBuffer();
	CPaperBurnCBuffer(const CPaperBurnCBuffer& buffer);
	virtual ~CPaperBurnCBuffer();

protected:
	PaperBurnCBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CPaperBurnCBuffer* Clone();

public:
	void SetBurn(float Burn)
	{
		m_BufferData.Burn = Burn;
	}

	void SetOutLineColor(const Vector3& OutLineColor)
	{
		m_BufferData.OutLineColor = OutLineColor;
	}

	void SetOutLineColorUNorm(float r, float g, float b)
	{
		m_BufferData.OutLineColor = Vector3(r, g, b);
	}

	void SetOutLineColor(unsigned char r, unsigned char g, unsigned char b)
	{
		m_BufferData.OutLineColor = Vector3(r / 255.f, g / 255.f, b / 255.f);
	}

	void SetInLineColor(const Vector3& InLineColor)
	{
		m_BufferData.InLineColor = InLineColor;
	}

	void SetInLineColorUNorm(float r, float g, float b)
	{
		m_BufferData.InLineColor = Vector3(r, g, b);
	}

	void SetInLineColor(unsigned char r, unsigned char g, unsigned char b)
	{
		m_BufferData.InLineColor = Vector3(r / 255.f, g / 255.f, b / 255.f);
	}

	void SetCenterColor(const Vector3& CenterColor)
	{
		m_BufferData.CenterColor = CenterColor;
	}

	void SetCenterColorUNorm(float r, float g, float b)
	{
		m_BufferData.CenterColor = Vector3(r, g, b);
	}

	void SetCenterColor(unsigned char r, unsigned char g, unsigned char b)
	{
		m_BufferData.CenterColor = Vector3(r / 255.f, g / 255.f, b / 255.f);
	}

	void SetBurnInverse(int Inverse)
	{
		m_BufferData.Inverse = Inverse;
	}

	void SetInFilter(float Inverse)
	{
		m_BufferData.InFilter = Inverse;
	}

	void SetCenterFilter(float Inverse)
	{
		m_BufferData.CenterFilter = Inverse;
	}

	void SetOutFilter(float Inverse)
	{
		m_BufferData.OutFilter = Inverse;
	}
};

