#pragma once

#include "../Ref.h"

/*
Vertex Shader : ������ ���� ��ȯ�ܰ踦 ó���ϴ� Shader
Pixel Shader : �ȼ��� �÷��� ���ϴ� Shader
HullShader : �׼����̼� �� �ܰ迡�� ������ ��� ��������, �󸶳� ���������� �����ϴ� Shader
�׼����̼� : ������ ����ȭ���Ѽ� �������� �޽ø� ����� �� �ְ� ���ִ� ���.
*/

class CShader :
    public CRef
{
    friend class CShaderManager;

protected:
    CShader();
    virtual ~CShader() = 0;

protected:
    Shader_Type m_Type;
    ID3D11InputLayout* m_InputLayout;

    // D3D11_INPUT_ELEMENT_DESC : ������ �Ѱ��� ��Ҹ� �����Ҷ� ����ϴ� ����ü�̴�.
    // ���� ��� ��ġ, ������ ���� ������ �� ����ü 2���� �̿��ؼ� ���� �ϳ��� ǥ���Ѵ�.
    std::vector<D3D11_INPUT_ELEMENT_DESC>   m_vecInputDesc;
    unsigned int        m_InputSize;

public:
    Shader_Type GetShaderType() const
    {
        return m_Type;
    }

    ID3D11InputLayout* GetInputLayout() const
    {
        return m_InputLayout;
    }

public:
    virtual bool Init();
    void AddInputDesc(const char* SemanticName, unsigned int SemanticIndex, DXGI_FORMAT Fmt,
        unsigned int InputSlot, unsigned int Size, D3D11_INPUT_CLASSIFICATION InputSlotClass,
        unsigned int InstanceDataStepRate);
    virtual bool CreateInputLayout();
    virtual void SetShader();
};

