#pragma once

#include "UI/WidgetWindow.h"
#include "UI/Image.h"
#include "UI/Button.h"
#include "UI/Text.h"
#include "UI/ProgressBar.h"
#include "UI/NumberWidget.h"

class CMainHUDWidget :
    public CWidgetWindow
{
    friend class CViewport;

protected:
    CMainHUDWidget();
    CMainHUDWidget(const CMainHUDWidget& widget);
    virtual ~CMainHUDWidget();

private:
    CSharedPtr<CText>   m_Text;
    float   m_Time;
    int m_Index;

    CSharedPtr<CProgressBar>    m_ProgressBar;
    float   m_TestPercent;

    CSharedPtr<CNumberWidget>   m_NumberWidget;
    float   m_TestNumberTime;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CMainHUDWidget* Clone();

private:
    void StartClick();
};

