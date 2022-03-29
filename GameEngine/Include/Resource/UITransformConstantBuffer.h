#pragma once
#include "ConstantBufferData.h"
class CUITransformConstantBuffer :
    public CConstantBufferData
{
public:
	CUITransformConstantBuffer();
	CUITransformConstantBuffer(const CUITransformConstantBuffer& buffer);
	virtual ~CUITransformConstantBuffer();

protected:
	TransformUICBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CUITransformConstantBuffer* Clone();

public:
	void SetWVP(const Matrix& matWVP)
	{
		m_BufferData.matWVP = matWVP;
		m_BufferData.matWVP.Transpose();
	}

	void SetTextureEnable(bool Enable)
	{
		m_BufferData.TextureEnable = Enable ? 1 : 0;
	}

	void SetAnimation2DEnable(bool Enable)
	{
		m_BufferData.Animation2D = Enable ? 1 : 0;
	}

	void SetUISize(const Vector2& Size)
	{
		m_BufferData.Size = Size;
	}
};

