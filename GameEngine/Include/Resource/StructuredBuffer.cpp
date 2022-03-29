
#include "StructuredBuffer.h"
#include "../Device.h"

CStructuredBuffer::CStructuredBuffer()	:
	m_Desc{},
	m_Buffer(nullptr),
	m_SRV(nullptr),
	m_UAV(nullptr),
	m_Size(0),
	m_Count(0),
	m_ConstantBufferType(0),
	m_Register(0),
	m_Dynamic(false)
{
}

CStructuredBuffer::CStructuredBuffer(const CStructuredBuffer& buffer)
{
	*this = buffer;

	DEVICE->CreateBuffer(&m_Desc, nullptr, &m_Buffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC	SRVDesc = {};

	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.FirstElement = 0;
	SRVDesc.BufferEx.Flags = 0;
	SRVDesc.BufferEx.NumElements = m_Count;

	DEVICE->CreateShaderResourceView(m_Buffer, &SRVDesc, &m_SRV);

	if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC	UAVDesc = {};

		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.FirstElement = 0;
		UAVDesc.Buffer.Flags = 0;
		UAVDesc.Buffer.NumElements = m_Count;

		DEVICE->CreateUnorderedAccessView(m_Buffer, &UAVDesc, &m_UAV);
	}

}

CStructuredBuffer::~CStructuredBuffer()
{
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
	SAFE_RELEASE(m_Buffer);
}

bool CStructuredBuffer::Init(const std::string& Name, unsigned int Size,
	unsigned int Count, int Register, bool Dynamic, int ConstantBufferType)
{
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
	SAFE_RELEASE(m_Buffer);

	m_Dynamic = Dynamic;

	m_Name = Name;
	m_Size = Size;
	m_Count = Count;
	m_Register = Register;
	m_ConstantBufferType = ConstantBufferType;

	m_Desc.ByteWidth = m_Size * m_Count;
	m_Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_Desc.StructureByteStride = m_Size;

	if (m_Dynamic)
	{
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_Desc.Usage = D3D11_USAGE_DYNAMIC;
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	else
	{
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		m_Desc.Usage = D3D11_USAGE_DEFAULT;
	}

	if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, &m_Buffer)))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC	SRVDesc = {};

	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.FirstElement = 0;
	SRVDesc.BufferEx.Flags = 0;
	SRVDesc.BufferEx.NumElements = m_Count;

	if (FAILED(DEVICE->CreateShaderResourceView(m_Buffer, &SRVDesc, &m_SRV)))
		return false;

	// 동적이 아닐 경우 UAV도 만들어준다.
	if (!m_Dynamic)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC	UAVDesc = {};

		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.FirstElement = 0;
		UAVDesc.Buffer.Flags = 0;
		UAVDesc.Buffer.NumElements = m_Count;

		if (FAILED(DEVICE->CreateUnorderedAccessView(m_Buffer, &UAVDesc, &m_UAV)))
			return false;
	}

	return true;
}

bool CStructuredBuffer::InitRead(const std::string& Name, unsigned int Size,
	unsigned int Count, int Register, int ConstantBufferType)
{
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
	SAFE_RELEASE(m_Buffer);

	m_Name = Name;
	m_Size = Size;
	m_Count = Count;
	m_Register = Register;
	m_ConstantBufferType = ConstantBufferType;

	m_Desc.ByteWidth = m_Size * m_Count;
	m_Desc.MiscFlags = 0;
	m_Desc.StructureByteStride = m_Size;
	m_Desc.Usage = D3D11_USAGE_STAGING;
	m_Desc.BindFlags = 0;
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, &m_Buffer)))
		return false;

	return true;
}

void CStructuredBuffer::UpdateBuffer(void* Data, int Size)
{
	if (!m_Dynamic)
		return;

	D3D11_MAPPED_SUBRESOURCE	Map = {};

	CONTEXT->Map(m_Buffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &Map);

	memcpy(Map.pData, Data, Size);

	CONTEXT->Unmap(m_Buffer, 0);
}

CStructuredBuffer* CStructuredBuffer::Clone()
{
	return new CStructuredBuffer(*this);
}

void CStructuredBuffer::SetShader(int Register)
{
	if (m_Dynamic)
	{
		if (m_ConstantBufferType & CBT_VERTEX)
			CONTEXT->VSSetShaderResources(Register, 1, &m_SRV);

		if (m_ConstantBufferType & CBT_PIXEL)
			CONTEXT->PSSetShaderResources(Register, 1, &m_SRV);

		if (m_ConstantBufferType & CBT_DOMAIN)
			CONTEXT->DSSetShaderResources(Register, 1, &m_SRV);

		if (m_ConstantBufferType & CBT_HULL)
			CONTEXT->HSSetShaderResources(Register, 1, &m_SRV);

		if (m_ConstantBufferType & CBT_GEOMETRY)
			CONTEXT->GSSetShaderResources(Register, 1, &m_SRV);

		if (m_ConstantBufferType & CBT_COMPUTE)
			CONTEXT->CSSetShaderResources(Register, 1, &m_SRV);
	}

	else
	{
		UINT	Count = -1;
		CONTEXT->CSSetUnorderedAccessViews(Register, 1, &m_UAV, &Count);
	}
}

void CStructuredBuffer::ResetShader(int Register)
{
	if (m_Dynamic)
	{
		ID3D11ShaderResourceView* SRV = nullptr;

		if (m_ConstantBufferType & CBT_VERTEX)
			CONTEXT->VSSetShaderResources(Register, 1, &SRV);

		if (m_ConstantBufferType & CBT_PIXEL)
			CONTEXT->PSSetShaderResources(Register, 1, &SRV);

		if (m_ConstantBufferType & CBT_DOMAIN)
			CONTEXT->DSSetShaderResources(Register, 1, &SRV);

		if (m_ConstantBufferType & CBT_HULL)
			CONTEXT->HSSetShaderResources(Register, 1, &SRV);

		if (m_ConstantBufferType & CBT_GEOMETRY)
			CONTEXT->GSSetShaderResources(Register, 1, &SRV);

		if (m_ConstantBufferType & CBT_COMPUTE)
			CONTEXT->CSSetShaderResources(Register, 1, &SRV);
	}

	else
	{
		UINT	Count = -1;
		ID3D11UnorderedAccessView* UAV = nullptr;
		CONTEXT->CSSetUnorderedAccessViews(Register, 1, &UAV, &Count);
	}
}

void CStructuredBuffer::SetShader(int Register, int ConstantBufferType)
{
	if (ConstantBufferType & CBT_VERTEX)
		CONTEXT->VSSetShaderResources(Register, 1, &m_SRV);

	if (ConstantBufferType & CBT_PIXEL)
		CONTEXT->PSSetShaderResources(Register, 1, &m_SRV);

	if (ConstantBufferType & CBT_DOMAIN)
		CONTEXT->DSSetShaderResources(Register, 1, &m_SRV);

	if (ConstantBufferType & CBT_HULL)
		CONTEXT->HSSetShaderResources(Register, 1, &m_SRV);

	if (ConstantBufferType & CBT_GEOMETRY)
		CONTEXT->GSSetShaderResources(Register, 1, &m_SRV);

	if (ConstantBufferType & CBT_COMPUTE)
		CONTEXT->CSSetShaderResources(Register, 1, &m_SRV);
}

void CStructuredBuffer::ResetShader(int Register, int ConstantBufferType)
{
	ID3D11ShaderResourceView* SRV = nullptr;

	if (ConstantBufferType & CBT_VERTEX)
		CONTEXT->VSSetShaderResources(Register, 1, &SRV);

	if (ConstantBufferType & CBT_PIXEL)
		CONTEXT->PSSetShaderResources(Register, 1, &SRV);

	if (ConstantBufferType & CBT_DOMAIN)
		CONTEXT->DSSetShaderResources(Register, 1, &SRV);

	if (ConstantBufferType & CBT_HULL)
		CONTEXT->HSSetShaderResources(Register, 1, &SRV);

	if (ConstantBufferType & CBT_GEOMETRY)
		CONTEXT->GSSetShaderResources(Register, 1, &SRV);

	if (ConstantBufferType & CBT_COMPUTE)
		CONTEXT->CSSetShaderResources(Register, 1, &SRV);
}

void CStructuredBuffer::CopyData(void* Data)
{
	D3D11_MAPPED_SUBRESOURCE	Map = {};

	CONTEXT->Map(m_Buffer, 0, D3D11_MAP::D3D11_MAP_READ, 0, &Map);

	memcpy(Data, Map.pData, m_Size * m_Count);

	CONTEXT->Unmap(m_Buffer, 0);
}

void CStructuredBuffer::CopyResource(CStructuredBuffer* Buffer)
{
	CONTEXT->CopyResource(Buffer->m_Buffer, m_Buffer);
}
