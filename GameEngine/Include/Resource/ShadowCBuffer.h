#pragma once
#include "ConstantBufferData.h"
class CShadowCBuffer :
    public CConstantBufferData
{
public:
	CShadowCBuffer();
	CShadowCBuffer(const CShadowCBuffer& buffer);
	virtual ~CShadowCBuffer();

protected:
	ShadowCBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CShadowCBuffer* Clone();

public:
	void SetShadowMatrix(const Matrix& matShadow)
	{
		m_BufferData.matShadow = matShadow;
	}

	void SetShadowInvVPMatrix(const Matrix& matInvVP)
	{
		m_BufferData.matShadowInvVP = matInvVP;
	}

	void SetShadowBias(float Bias)
	{
		m_BufferData.ShadowBias = Bias;
	}
};

