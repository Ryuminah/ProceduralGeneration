#pragma once

#include "ConstantBufferData.h"

class CLandScapeCBuffer :
    public CConstantBufferData
{
public:
	CLandScapeCBuffer();
	CLandScapeCBuffer(const CLandScapeCBuffer& buffer);
	virtual ~CLandScapeCBuffer();

protected:
	LandScapeCBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CLandScapeCBuffer* Clone();

public:
	void SetDetailLevel(float Level)
	{
		m_BufferData.DetailLevel = Level;
	}

	void SetSplatCount(int Count)
	{
		m_BufferData.SplatCount = Count;
	}
};

