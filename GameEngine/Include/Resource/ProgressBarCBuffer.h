#pragma once
#include "ConstantBufferData.h"
class CProgressBarCBuffer :
    public CConstantBufferData
{
public:
	CProgressBarCBuffer();
	CProgressBarCBuffer(const CProgressBarCBuffer& buffer);
	virtual ~CProgressBarCBuffer();

protected:
	ProgressBarCBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CProgressBarCBuffer* Clone();

public:
	void SetPercent(float Percent)
	{
		m_BufferData.Percent = Percent;
	}

	void SetDir(ProgressBar_Dir Dir)
	{
		m_BufferData.Dir = (int)Dir;
	}
};

