
#include "StartHUDWidget.h"
#include "Device.h"
#include "Engine.h"
#include "../SceneMode/MainScene.h"
#include "../SceneMode/LoadingScene.h"
#include "Scene/SceneManager.h"

CStartHUDWidget::CStartHUDWidget()
{
}

CStartHUDWidget::CStartHUDWidget(const CStartHUDWidget& widget) :
	CWidgetWindow(widget)
{
}

CStartHUDWidget::~CStartHUDWidget()
{
}

bool CStartHUDWidget::Init()
{
	CWidgetWindow::Init();

	Resolution	RS = CDevice::GetInst()->GetResolution();

	Vector2	Center;
	Center.x = RS.Width / 2.f;
	Center.y = RS.Height / 2.f;

	CButton* Button = CreateWidget<CButton>("StartButton");

	Button->SetPos(Center.x - 100.f, Center.y + 50.f);
	Button->SetSize(200.f, 50.f);
	Button->SetStateTexture(Button_State::Normal, "StartButton", TEXT("Start.png"));
	Button->SetStateTexture(Button_State::MouseOn, "StartButton", TEXT("Start.png"));
	Button->SetStateTexture(Button_State::Click, "StartButton", TEXT("Start.png"));

	Button->SetStateTint(Button_State::Normal, 0.8f, 0.8f, 0.8f, 1.f);
	Button->SetStateTint(Button_State::MouseOn, 1.f, 1.f, 1.f, 1.f);
	Button->SetStateTint(Button_State::Click, 0.6f, 0.6f, 0.6f, 1.f);

	Button->SetClickCallback<CStartHUDWidget>(this, &CStartHUDWidget::StartClick);

	Button->SetInteractionSound(Button_Sound_State::MouseOn, "ButtonMouseOn");
	Button->SetInteractionSound(Button_Sound_State::Click, "ButtonClick");



	Button = CreateWidget<CButton>("ExitButton");

	Button->SetPos(Center.x - 100.f, Center.y - 100.f);
	Button->SetSize(200.f, 50.f);
	Button->SetStateTexture(Button_State::Normal, "ExitButton", TEXT("End.png"));
	Button->SetStateTexture(Button_State::MouseOn, "ExitButton", TEXT("End.png"));
	Button->SetStateTexture(Button_State::Click, "ExitButton", TEXT("End.png"));

	Button->SetStateTint(Button_State::Normal, 0.8f, 0.8f, 0.8f, 1.f);
	Button->SetStateTint(Button_State::MouseOn, 1.f, 1.f, 1.f, 1.f);
	Button->SetStateTint(Button_State::Click, 0.6f, 0.6f, 0.6f, 1.f);

	Button->SetClickCallback<CStartHUDWidget>(this, &CStartHUDWidget::ExitClick);

	Button->SetInteractionSound(Button_Sound_State::MouseOn, "ButtonMouseOn");
	Button->SetInteractionSound(Button_Sound_State::Click, "ButtonClick");

	return true;
}

void CStartHUDWidget::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);
}

void CStartHUDWidget::PostUpdate(float DeltaTime)
{
	CWidgetWindow::PostUpdate(DeltaTime);
}

void CStartHUDWidget::Render()
{
	CWidgetWindow::Render();
}

CStartHUDWidget* CStartHUDWidget::Clone()
{
	return new CStartHUDWidget(*this);
}

void CStartHUDWidget::StartClick()
{
	// 다음 장면으로 넘어간다.
	CSceneManager::GetInst()->CreateNextScene();

	CSceneManager::GetInst()->SetSceneMode<CLoadingScene>(false);
}

void CStartHUDWidget::ExitClick()
{
	CEngine::GetInst()->Exit();
}
