
#include "ProgressBar.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Shader.h"
#include "../Input.h"
#include "../Resource/UITransformConstantBuffer.h"
#include "WidgetWindow.h"
#include "../Component/WidgetComponent.h"

CProgressBar::CProgressBar()	:
	m_Percent(1.f),
	m_Dir(ProgressBar_Dir::RightToLeft),
	m_ProgressBarCBuffer(nullptr)
{
	m_BackInfo.Tint = Vector4(0.3f, 0.3f, 0.3f, 1.f);
}

CProgressBar::CProgressBar(const CProgressBar& widget)
{
	m_BarInfo = widget.m_BarInfo;
	m_BackInfo = widget.m_BackInfo;

	if (widget.m_ProgressBarCBuffer)
		m_ProgressBarCBuffer = widget.m_ProgressBarCBuffer->Clone();
}

CProgressBar::~CProgressBar()
{
	SAFE_DELETE(m_ProgressBarCBuffer);
}

bool CProgressBar::Init()
{
	CWidget::Init();

	m_BackShader = CShaderManager::GetInst()->FindShader("UIImageShader");
	m_Shader = CShaderManager::GetInst()->FindShader("ProgressBarShader");

	m_ProgressBarCBuffer = new CProgressBarCBuffer;

	m_ProgressBarCBuffer->Init();

	m_ProgressBarCBuffer->SetPercent(m_Percent);
	m_ProgressBarCBuffer->SetDir(m_Dir);

	return true;
}

void CProgressBar::Update(float DeltaTime)
{
	CWidget::Update(DeltaTime);
}

void CProgressBar::PostUpdate(float DeltaTime)
{
	CWidget::PostUpdate(DeltaTime);
}

void CProgressBar::Render()
{
	m_TintCBuffer->SetTint(m_BackInfo.Tint);

	if (m_BackInfo.Texture)
		m_TransformCBuffer->SetTextureEnable(true);

	else
		m_TransformCBuffer->SetTextureEnable(false);

	m_TransformCBuffer->UpdateCBuffer();

	m_TintCBuffer->UpdateCBuffer();

	m_BackShader->SetShader();

	if (m_BackInfo.Texture)
		m_BackInfo.Texture->SetShader(0, TST_PIXEL);

	m_Mesh->Render();

	if (m_BackInfo.Texture)
		m_BackInfo.Texture->ResetShader(0, TST_PIXEL);


	// 위젯컴포넌트를 이용한 월드출력일 경우 z를 Back보다 아주 미세하게 앞으로
	// 올 수 있도록 조절해야 한다.
	if (m_Owner->GetWidgetComponent())
	{
		if (m_Owner->GetWidgetComponent()->GetWidgetSpace() == WidgetComponent_Space::Space2D)
		{
			UpdateWorldTransformZ();
		}
	}

	m_ProgressBarCBuffer->UpdateCBuffer();
	m_TintCBuffer->SetTint(m_BarInfo.Tint);

	if (m_BarInfo.Texture)
		m_TransformCBuffer->SetTextureEnable(true);

	else
		m_TransformCBuffer->SetTextureEnable(false);

	m_TransformCBuffer->UpdateCBuffer();

	m_TintCBuffer->UpdateCBuffer();

	m_Shader->SetShader();

	if (m_BarInfo.Texture)
		m_BarInfo.Texture->SetShader(0, TST_PIXEL);

	m_Mesh->Render();

	if (m_BarInfo.Texture)
		m_BarInfo.Texture->ResetShader(0, TST_PIXEL);
}

CProgressBar* CProgressBar::Clone()
{
	return new CProgressBar(*this);
}

void CProgressBar::CollisionMouse(const Vector2& MousePos, float DeltaTime)
{
	CWidget::CollisionMouse(MousePos, DeltaTime);
}

void CProgressBar::CollisionReleaseMouse(const Vector2& MousePos, float DeltaTime)
{
	CWidget::CollisionReleaseMouse(MousePos, DeltaTime);
}

void CProgressBar::SetTexture(CTexture* Texture)
{
	m_BarInfo.Texture = Texture;
}

bool CProgressBar::SetTexture(const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
		return false;

	m_BarInfo.Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

bool CProgressBar::SetTextureFullPath(
	const std::string& Name, const TCHAR* FullPath)
{
	if (!m_Scene->GetResource()->LoadTextureFullPath(Name, FullPath))
		return false;

	m_BarInfo.Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

bool CProgressBar::SetTexture(const std::string& Name,
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	if (!m_Scene->GetResource()->LoadTexture(Name, vecFileName, PathName))
		return false;

	m_BarInfo.Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

bool CProgressBar::SetTextureFullPath(
	const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
	if (!m_Scene->GetResource()->LoadTextureFullPath(Name, vecFullPath))
		return false;

	m_BarInfo.Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

bool CProgressBar::SetTextureArray(
	const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
	const std::string& PathName)
{
	if (!m_Scene->GetResource()->LoadTextureArray(Name, vecFileName, PathName))
		return false;

	m_BarInfo.Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

bool CProgressBar::SetTextureArrayFullPath(
	const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
	if (!m_Scene->GetResource()->LoadTextureArrayFullPath(Name, vecFullPath))
		return false;

	m_BarInfo.Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

void CProgressBar::SetTint(float r, float g,
	float b, float a)
{
	m_BarInfo.Tint = Vector4(r, g, b, a);
}

void CProgressBar::SetTint(unsigned char r,
	unsigned char g, unsigned char b, unsigned char a)
{
	m_BarInfo.Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CProgressBar::SetTint(const Vector4& Color)
{
	m_BarInfo.Tint = Color;
}

void CProgressBar::SetBackTexture(CTexture* Texture)
{
	m_BackInfo.Texture = Texture;
}

bool CProgressBar::SetBackTexture(const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
		return false;

	m_BackInfo.Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

bool CProgressBar::SetBackTextureFullPath(
	const std::string& Name, const TCHAR* FullPath)
{
	if (!m_Scene->GetResource()->LoadTextureFullPath(Name, FullPath))
		return false;

	m_BackInfo.Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

bool CProgressBar::SetBackTexture(const std::string& Name,
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	if (!m_Scene->GetResource()->LoadTexture(Name, vecFileName, PathName))
		return false;

	m_BackInfo.Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

bool CProgressBar::SetBackTextureFullPath(
	const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
	if (!m_Scene->GetResource()->LoadTextureFullPath(Name, vecFullPath))
		return false;

	m_BackInfo.Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

bool CProgressBar::SetBackTextureArray(
	const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
	const std::string& PathName)
{
	if (!m_Scene->GetResource()->LoadTextureArray(Name, vecFileName, PathName))
		return false;

	m_BackInfo.Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

bool CProgressBar::SetBackTextureArrayFullPath(
	const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
	if (!m_Scene->GetResource()->LoadTextureArrayFullPath(Name, vecFullPath))
		return false;

	m_BackInfo.Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

void CProgressBar::SetBackTint(float r, float g,
	float b, float a)
{
	m_BackInfo.Tint = Vector4(r, g, b, a);
}

void CProgressBar::SetBackTint(unsigned char r,
	unsigned char g, unsigned char b, unsigned char a)
{
	m_BackInfo.Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CProgressBar::SetBackTint(const Vector4& Color)
{
	m_BackInfo.Tint = Color;
}

