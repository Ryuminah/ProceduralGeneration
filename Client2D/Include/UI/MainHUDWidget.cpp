
#include "MainHUDWidget.h"

CMainHUDWidget::CMainHUDWidget()
{
}

CMainHUDWidget::CMainHUDWidget(const CMainHUDWidget& widget)	:
	CWidgetWindow(widget)
{
}

CMainHUDWidget::~CMainHUDWidget()
{
}

bool CMainHUDWidget::Init()
{
	CWidgetWindow::Init();

	CImage* Back = CreateWidget<CImage>("TestImage");

	Back->SetPos(100.f, 50.f);
	Back->SetSize(200.f, 200.f);
	Back->SetTexture("TestImage", TEXT("teemo.png"));
	Back->SetZOrder(1);

	Back = CreateWidget<CImage>("TestImage");

	Back->SetPos(150.f, 50.f);
	Back->SetSize(200.f, 200.f);
	Back->SetTexture("TestImage", TEXT("teemo.png"));

	CButton* Button = CreateWidget<CButton>("Button");

	Button->SetPos(500.f, 300.f);
	Button->SetSize(200.f, 50.f);
	Button->SetStateTexture(Button_State::Normal, "StartButton", TEXT("Start.png"));
	Button->SetStateTexture(Button_State::MouseOn, "StartButton", TEXT("Start.png"));
	Button->SetStateTexture(Button_State::Click, "StartButton", TEXT("Start.png"));

	Button->SetStateTint(Button_State::Normal, 0.8f, 0.8f, 0.8f, 1.f);
	Button->SetStateTint(Button_State::MouseOn, 1.f, 1.f, 1.f, 1.f);
	Button->SetStateTint(Button_State::Click, 0.6f, 0.6f, 0.6f, 1.f);

	Button->SetClickCallback<CMainHUDWidget>(this, &CMainHUDWidget::StartClick);

	Button->SetInteractionSound(Button_Sound_State::MouseOn, "ButtonMouseOn");
	Button->SetInteractionSound(Button_Sound_State::Click, "ButtonClick");

	m_Text = CreateWidget<CText>("Text");

	m_Text->SetText(TEXT(""));
	m_Text->SetPos(100.f, 100.f);
	m_Text->SetSize(300.f, 50.f);
	m_Text->SetFontSize(30.f);
	m_Text->SetShadowEnable(true);
	m_Text->SetShadowColor(1.f, 1.f, 0.f);
	m_Text->SetShadowAlphaEnable(true);
	m_Text->SetShadowOpacity(0.5f);

	m_Time = 0.f;
	m_Index = 0;

	m_ProgressBar = CreateWidget<CProgressBar>("ProgressBar");

	m_ProgressBar->SetPos(50.f, 630.f);
	m_ProgressBar->SetSize(300.f, 50.f);
	m_ProgressBar->SetTint(1.f, 1.f, 1.f, 0.5f);
	m_ProgressBar->SetTexture("HPBar", TEXT("HPBar.png"));
	//m_ProgressBar->SetPercent(0.5f);

	m_TestPercent = 1.f;

	m_NumberWidget = CreateWidget<CNumberWidget>("NumberWidget");

	m_NumberWidget->SetPos(800.f, 650.f);
	m_NumberWidget->SetNumberSize(50.f, 50.f);
	m_NumberWidget->SetTexture("Number", TEXT("Number.png"));
	m_NumberWidget->SetNumber(1234);
	m_NumberWidget->SetFrameCount(10, 2);
	
	m_TestNumberTime = 0.f;

	//m_ProgressBar->SetBackTint(1.f, 1.f, 1.f, 1.f);
	//m_ProgressBar->SetTexture("TestImage", TEXT("teemo.png"));
	//m_ProgressBar->SetZOrder(1);
	

	return true;
}

void CMainHUDWidget::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);

	m_TestNumberTime += DeltaTime;

	if (m_TestNumberTime >= 0.5f)
	{
		m_TestNumberTime = 0.f;
		m_NumberWidget->AddNumber(1);
	}

	static const TCHAR* Text = TEXT("한글도 제대로 되냐?");

	static bool Test = false;
	static bool Test1 = false;

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		Test = true;

	else if (Test)
	{
		Test1 = true;
		Test = false;
	}

	if (Test1)
	{

		m_Time += DeltaTime;

		if (m_Time >= 0.2f)
		{
			m_Time -= 0.2f;

			int	Length = lstrlen(Text);

			if (m_Index < Length)
			{
				TCHAR	TestText[2] = {};
				TestText[0] = Text[m_Index];
				m_Text->AddText(TestText);
				++m_Index;
			}
		}
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_TestPercent += 0.5f * DeltaTime;

		if (m_TestPercent > 1.f)
			m_TestPercent = 1.f;

		m_ProgressBar->SetPercent(m_TestPercent);
	}


	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_TestPercent -= 0.5f * DeltaTime;

		if (m_TestPercent < 0.f)
			m_TestPercent = 0.f;

		m_ProgressBar->SetPercent(m_TestPercent);
	}

}

void CMainHUDWidget::PostUpdate(float DeltaTime)
{
	CWidgetWindow::PostUpdate(DeltaTime);
}

void CMainHUDWidget::Render()
{
	CWidgetWindow::Render();
}

CMainHUDWidget* CMainHUDWidget::Clone()
{
	return new CMainHUDWidget(*this);
}

void CMainHUDWidget::StartClick()
{
}
