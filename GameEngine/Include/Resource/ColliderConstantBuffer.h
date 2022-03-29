#pragma once
#include "ConstantBufferData.h"
class CColliderConstantBuffer :
    public CConstantBufferData
{
public:
	CColliderConstantBuffer();
	CColliderConstantBuffer(const CColliderConstantBuffer& buffer);
	virtual ~CColliderConstantBuffer();

protected:
	ColliderCBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CColliderConstantBuffer* Clone();

public:
	void SetColor(const Vector4& Color)
	{
		m_BufferData.Color = Color;
	}
};

