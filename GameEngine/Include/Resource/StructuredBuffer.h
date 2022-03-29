#pragma once

#include "../GameEngine.h"

class CStructuredBuffer
{
public:
	CStructuredBuffer();
	CStructuredBuffer(const CStructuredBuffer& buffer);
	~CStructuredBuffer();

private:
	std::string         m_Name;
	D3D11_BUFFER_DESC   m_Desc;
	ID3D11Buffer* m_Buffer;

	ID3D11ShaderResourceView* m_SRV;
	ID3D11UnorderedAccessView* m_UAV;

	unsigned int    m_Size;
	unsigned int    m_Count;
	int             m_ConstantBufferType;
	bool            m_Dynamic;
	int				m_Register;

public:
	const std::string& GetName()	const
	{
		return m_Name;
	}

	ID3D11Buffer* GetBuffer()	const
	{
		return m_Buffer;
	}

public:
	bool Init(const std::string& Name, unsigned int Size, unsigned int Count,
		int Register, bool Dynamic = false,
		int ConstantBufferType = CBT_COMPUTE);
	bool InitRead(const std::string& Name, unsigned int Size, unsigned int Count,
		int Register, int ConstantBufferType = CBT_COMPUTE);
	void UpdateBuffer(void* Data, int Size);
	CStructuredBuffer* Clone();
	void SetShader(int Register);
	void ResetShader(int Register);
	void SetShader(int Register, int ConstantBufferType);
	void ResetShader(int Register, int ConstantBufferType);
	void CopyData(void* Data);
	void CopyResource(CStructuredBuffer* Buffer);
};

