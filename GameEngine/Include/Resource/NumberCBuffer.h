#pragma once
#include "ConstantBufferData.h"
class CNumberCBuffer :
    public CConstantBufferData
{
public:
	CNumberCBuffer();
	CNumberCBuffer(const CNumberCBuffer& buffer);
	virtual ~CNumberCBuffer();

protected:
	NumberCBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CNumberCBuffer* Clone();

public:
	void SetNumberUV(const Vector2& StartUV, const Vector2& EndUV)
	{
		m_BufferData.StartUV = StartUV;
		m_BufferData.EndUV = EndUV;
	}
};

