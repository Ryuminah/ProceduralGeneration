#pragma once

#include "ConstantBufferData.h"

class CRenderTargetCBuffer :
    public CConstantBufferData
{
public:
	CRenderTargetCBuffer();
	CRenderTargetCBuffer(const CRenderTargetCBuffer& buffer);
	virtual ~CRenderTargetCBuffer();

protected:
	RenderTargetCBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CRenderTargetCBuffer* Clone();

public:
	void SetWVP(const Matrix& matWVP)
	{
		m_BufferData.matWVP = matWVP;
	}
};

