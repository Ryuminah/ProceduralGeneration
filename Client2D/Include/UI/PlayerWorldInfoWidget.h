#pragma once

#include "UI\WidgetWindow.h"
#include "UI/Text.h"
#include "UI/ProgressBar.h"
#include "UI/Button.h"
#include "UI/NumberWidget.h"

class CPlayerWorldInfoWidget :
    public CWidgetWindow
{
public:
    CPlayerWorldInfoWidget();
    CPlayerWorldInfoWidget(const CPlayerWorldInfoWidget& widget);
    virtual ~CPlayerWorldInfoWidget();

private:
    CSharedPtr<CText>   m_NameText;
    CSharedPtr<CProgressBar>    m_HPBar;
    CSharedPtr<CNumberWidget>    m_NumberWidget;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CPlayerWorldInfoWidget* Clone();

public:
    void ClickCallback();
};

