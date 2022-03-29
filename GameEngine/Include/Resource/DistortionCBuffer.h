#pragma once
#include "ConstantBufferData.h"
class CDistortionCBuffer :
    public CConstantBufferData
{
public:
	CDistortionCBuffer();
	CDistortionCBuffer(const CDistortionCBuffer& buffer);
	virtual ~CDistortionCBuffer();

protected:
	Distortion2DCBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CDistortionCBuffer* Clone();

public:
	void SetWeight(float Weight)
	{
		m_BufferData.Weight = Weight;
	}

	void SetSelf(bool Self)
	{
		m_BufferData.Self = Self ? 1 : 0;
	}

	void SetSpeed(float Speed)
	{
		m_BufferData.Speed = Speed;
	}
};

