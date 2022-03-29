#include "LoadingUI.h"

CLoadingUI::CLoadingUI()
{
}

CLoadingUI::CLoadingUI(const CLoadingUI& widget)
{
}

CLoadingUI::~CLoadingUI()
{
}

bool CLoadingUI::Init()
{
	CWidgetWindow::Init();

	CImage* Back = CreateWidget<CImage>("LoadingBack");

	Back->SetPos(0.f, 0.f);
	Back->SetSize(1280.f, 720.f);
	Back->SetTexture("LoadingBack", TEXT("LoadingBack.jpg"));
	Back->SetZOrder(0);

	m_LoadingBar = CreateWidget<CProgressBar>("LoadingBar");

	m_LoadingBar->SetPos(1280.f / 2.f - 400.f, 70.f);
	m_LoadingBar->SetSize(800.f, 30.f);
	m_LoadingBar->SetTint(1.f, 1.f, 0.f, 1.f);
	m_LoadingBar->SetPercent(0.f);
	m_LoadingBar->SetZOrder(1);

	m_LoadingPercent = 0.f;

	return true;
}

void CLoadingUI::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);
}

void CLoadingUI::PostUpdate(float DeltaTime)
{
	CWidgetWindow::PostUpdate(DeltaTime);
}

void CLoadingUI::Render()
{
	CWidgetWindow::Render();
}

CLoadingUI* CLoadingUI::Clone()
{
	return new CLoadingUI(*this);
}
