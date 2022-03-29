#pragma once

#include "UI\WidgetWindow.h"
#include "UI/Button.h"

class CStartHUDWidget :
    public CWidgetWindow
{
    friend class CViewport;

protected:
    CStartHUDWidget();
    CStartHUDWidget(const CStartHUDWidget& widget);
    virtual ~CStartHUDWidget();

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CStartHUDWidget* Clone();

private:
    void StartClick();
    void ExitClick();
};

