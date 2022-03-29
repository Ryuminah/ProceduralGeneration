#pragma once

#include "Shader.h"

class CComputeShader :
	public CShader
{
	friend class CShaderManager;

protected:
	CComputeShader();
	virtual ~CComputeShader();

private:
	ID3D11ComputeShader* m_CS;
	ID3DBlob* m_CSBlob;

public:
	bool LoadComputeShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
		const std::string& PathName);

public:
	virtual bool Init();
	virtual void SetShader();
	void Excute(unsigned int x, unsigned int y, unsigned int z);
};

