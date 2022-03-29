#pragma once

#include "Shader.h"

/*
Vertex Shader, Pixel Shader, Domain Shader, Hull Shader, Geometry Shader
*/

class CGraphicShader :
	public CShader
{
	friend class CShaderManager;

protected:
	CGraphicShader();
	virtual ~CGraphicShader();

protected:
	// ID3DBlob 은 Shader 코드를 저장해두는 역할을 한다.
	ID3D11VertexShader* m_pVS;
	ID3DBlob* m_pVSBlob;

	ID3D11PixelShader* m_pPS;
	ID3DBlob* m_pPSBlob;

	ID3D11HullShader* m_pHS;
	ID3DBlob* m_pHSBlob;

	ID3D11DomainShader* m_pDS;
	ID3DBlob* m_pDSBlob;

	ID3D11GeometryShader* m_pGS;
	ID3DBlob* m_pGSBlob;

public:
	bool LoadVertexShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
		const std::string& PathName);
	bool LoadPixelShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
		const std::string& PathName);
	bool LoadDomainShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
		const std::string& PathName);
	bool LoadHullShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
		const std::string& PathName);
	bool LoadGeometryShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
		const std::string& PathName);

public:
	virtual bool Init();
	virtual bool CreateInputLayout();
	virtual void SetShader();
};

