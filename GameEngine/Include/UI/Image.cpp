
#include "Image.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Shader.h"
#include "../Resource/UITransformConstantBuffer.h"

CImage::CImage()
{
}

CImage::CImage(const CImage& widget)	:
	CWidget(widget)
{
	m_Texture = widget.m_Texture;
}

CImage::~CImage()
{
}

void CImage::SetTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
		return;

	m_Texture = m_Scene->GetResource()->FindTexture(Name);

	if (m_Texture)
		m_TransformCBuffer->SetTextureEnable(true);

	else
		m_TransformCBuffer->SetTextureEnable(false);
}

bool CImage::Init()
{
	CWidget::Init();

	m_Shader = CShaderManager::GetInst()->FindShader("UIImageShader");

	return true;
}

void CImage::Update(float DeltaTime)
{
	CWidget::Update(DeltaTime);
}

void CImage::PostUpdate(float DeltaTime)
{
	CWidget::PostUpdate(DeltaTime);
}

void CImage::Render()
{
	CWidget::Render();

	if (m_Texture)
		m_Texture->SetShader(0, TST_PIXEL);

	m_Mesh->Render();

	if (m_Texture)
		m_Texture->ResetShader(0, TST_PIXEL);
}

CImage* CImage::Clone()
{
	return new CImage(*this);
}
