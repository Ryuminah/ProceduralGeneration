
#include "MouseWidget.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Shader.h"
#include "../Resource/UITransformConstantBuffer.h"
#include "../Resource/ResourceManager.h"
#include "../Input.h"

CMouseWidget::CMouseWidget()
{
}

CMouseWidget::CMouseWidget(const CMouseWidget& widget) :
	CWidget(widget)
{
	m_Texture = widget.m_Texture;

	if (widget.m_Animation)
	{
		m_Animation = widget.m_Animation->Clone();
	}
}

CMouseWidget::~CMouseWidget()
{
}

void CMouseWidget::SetTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
		return;

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	if (m_Texture)
		m_TransformCBuffer->SetTextureEnable(true);

	else
		m_TransformCBuffer->SetTextureEnable(false);
}

bool CMouseWidget::Init()
{
	CWidget::Init();

	m_Shader = CShaderManager::GetInst()->FindShader("MouseShader");

	return true;
}

void CMouseWidget::Update(float DeltaTime)
{
	CWidget::Update(DeltaTime);

	if (m_Animation)
		m_Animation->Update(DeltaTime);

	m_Pos = CInput::GetInst()->GetMousePos();
}

void CMouseWidget::PostUpdate(float DeltaTime)
{
	CWidget::PostUpdate(DeltaTime);
}

void CMouseWidget::Render()
{
	CWidget::Render();

	if (m_Animation)
		m_Animation->SetShader();

	else
	{
		if (m_Texture)
			m_Texture->SetShader(0, TST_PIXEL);
	}

	m_Mesh->Render();

	if (m_Animation)
		m_Animation->ResetShader();

	else
	{
		if (m_Texture)
			m_Texture->ResetShader(0, TST_PIXEL);
	}
}

CMouseWidget* CMouseWidget::Clone()
{
	return new CMouseWidget(*this);
}
