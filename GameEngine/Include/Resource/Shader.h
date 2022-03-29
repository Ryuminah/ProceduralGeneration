#pragma once

#include "../Ref.h"

/*
Vertex Shader : 정점의 공간 변환단계를 처리하는 Shader
Pixel Shader : 픽셀의 컬러를 정하는 Shader
HullShader : 테셀레이션 전 단계에서 도형을 어떻게 분할할지, 얼마나 분할할지를 결정하는 Shader
테셀레이션 : 도형을 세분화시켜서 디테일한 메시를 출력할 수 있게 해주는 기능.
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

    // D3D11_INPUT_ELEMENT_DESC : 정점의 한가지 요소를 정의할때 사용하는 구조체이다.
    // 예를 들어 위치, 색상을 갖는 정점은 이 구조체 2개를 이용해서 정점 하나를 표현한다.
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

