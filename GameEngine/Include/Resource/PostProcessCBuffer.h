#pragma once
#include "ConstantBufferData.h"
class CPostProcessCBuffer :
    public CConstantBufferData
{
public:
	CPostProcessCBuffer();
	CPostProcessCBuffer(const CPostProcessCBuffer& buffer);
	virtual ~CPostProcessCBuffer();

protected:
	PostProcessCBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CPostProcessCBuffer* Clone();

public:
	void SetBlur(bool Enable)
	{
		m_BufferData.Blur = Enable ? 1 : 0;
	}

	void SetDistortion(bool Enable)
	{
		m_BufferData.Distortion = Enable ? 1 : 0;
	}
};

